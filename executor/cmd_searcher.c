/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_searcher.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	**get_args(char *cmd);

// char	**envp_str(t_envp *datas)
// {
// 	char	**res;
// 	t_envp	*actual;
// 	int		i;

// 	i = 0;
// 	actual = datas;
// 	while (actual != NULL)
// 	{
// 		i++;
// 		actual = actual->next;
// 	}
// 	res = malloc((i + 1) * sizeof(char *));
// 	if (res == NULL)
// 		return (0);
// 	actual = datas;
// 	i = 0;
// 	while (actual != NULL)
// 	{
// 		res[i] = actual->data;
// 		actual = actual->next;
// 		i++;
// 	}
// 	res[i] = NULL;
// 	return (res);
// }

void	add_pid(pid_t **pids, pid_t newpid)
{
	int		i;
	pid_t	*actual;

	i = 0;
	actual = *pids;
	while (actual[i] != 0)
		i++;
	actual[i] = newpid;
	actual[i + 1] = 0;
}

void	end_pids(pid_t **pids)
{
	int		i;
	int		status;
	pid_t	*actual;

	i = 0;
	actual = *pids;
	while (actual[i] != 0)
	{
		waitpid(actual[i], &status, 0);
		i++;
	}
	free(actual);
}

void	last_executor(char *cmd_path, t_main *main, char **envp, int tube, pid_t **pids)
{
	pid_t	pid;
	//int		status;
	int		i;

	i = 0;
	pid = fork();
	if (pid == 0)
	{
		while (main->cmd_info->argv[i])
		{
			printf("ARG %s\n", main->cmd_info->argv[i]);
			i++;
		}
		i = 0;
		printf("TEST %s %d\n", cmd_path, main->cmd_info->outfile_fd);
		if (tube != -1)
		{
			printf("PRINTF\n");
			dup2(tube, STDIN_FILENO);
			close(tube);
		}
		if (main->cmd_info->outfile_fd != -1)
		{
			printf("PRINTF2\n");
			dup2(main->cmd_info->outfile_fd, STDOUT_FILENO);
			printf("PRINTF3\n");
			//close(main->cmd_info->outfile_fd);
		}
		if (execve(cmd_path, (char *const *)main->cmd_info->argv, envp) == -1)
		{
			close(main->cmd_info->outfile_fd);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		// while (main->cmd_info->argv[i])
		// 	free(main->cmd_info->argv[i++]);
		// free(main->cmd_info->argv);
	}
	else
	{
    while (main->cmd_info->argv[i])
			free(main->cmd_info->argv[i++]);
		free(main->cmd_info->argv);
		// if (tube != -1)
		// 	close(tube);
		// if (file != -1)
		// 	close(file);
		add_pid(pids, pid);
		//waitpid(pid, &status, 0);
	}
}

char	*paths_searcher(char *cmd, char *cmd_path, char *paths)
{
	char	*tmp;
	char	**res;
	int		i;

	tmp = ft_strjoin(paths, "/");
	i = 0;
	cmd_path = ft_strjoin(tmp, cmd);
	free(tmp);
	res = ft_split(cmd_path, ' ');
	free(cmd_path);
	cmd_path = ft_strdup(res[0]);
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (cmd_path);
}

void	lcmd_searcher(t_main *main, char **envp, int tube, pid_t **pids)
{
	int		i;
	char	*cmd_path;
	int		cmdopener;
	char	**tmp;

	i = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
	{
		tmp = ft_split(main->cmd_info->cmd, ' ');
		cmd_path = ft_strdup(tmp[0]);
		while (tmp[i])
			free(tmp[i++]);
		free(tmp);
		last_executor(cmd_path, main, envp, tube, pids);
		free(cmd_path);
	}
	i = 0;
	while (main->cmds_paths->paths[i])
	{
		cmd_path = paths_searcher(main->cmd_info->cmd, cmd_path, main->cmds_paths->paths[i]);
		cmdopener = open(cmd_path, O_RDONLY);
		if (cmdopener != -1)
		{
			close(cmdopener);
			last_executor(cmd_path, main, envp, tube, pids);
			free(cmd_path);
			break ;
		}
		close(cmdopener);
		free(cmd_path);
		i++;
	}
}

int	cmd_executor(char *cmd_path, char **args, char **envp, int file, pid_t **pids)
{
	pid_t	pid;
	//int		status;
	int		tube[2];
	int	i;

	i = 0;
	pipe(tube);
	pid = fork();
	if (pid == 0)
	{
		close(tube[0]);
		if (file != -1)
			dup2(file, STDIN_FILENO);
		printf("ON RENTRE %s\n", cmd_path);
		dup2(tube[1], STDOUT_FILENO);
		close(tube[1]);
		close(file);

		if (execve(cmd_path, (char *const *)args, envp) == -1)
		{
			close(tube[1]);
			close(file);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		// while (args[i])
		// 	free(args[i++]);
		// free(args);
	}
	else
	{
		while (args[i])
			free(args[i++]);
		free(args);
		close(tube[1]);
		close(file);
		add_pid(pids, pid);
		//waitpid(pid, &status, 0);
	}
	return (tube[0]);
}

int	cmd_searcher(t_main *main, char **envp, int file, char **args, pid_t **pids)
{
	int		i;
	char	*cmd_path;
	int		tube;
	int		cmdopener;
	char	**tmp;

	i = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
	{
		tmp = ft_split(main->cmd_info->cmd, ' ');
		cmd_path = ft_strdup(tmp[0]);
		while (tmp[i])
			free(tmp[i++]);
		free(tmp);
		tube = cmd_executor(cmd_path, args, envp, file, pids);
		free(cmd_path);
	}
	i = 0;
	while (main->cmds_paths->paths[i])
	{
		cmd_path = paths_searcher(main->cmd_info->cmd, cmd_path, main->cmds_paths->paths[i]);
		cmdopener = open(cmd_path, O_RDONLY);
		if (cmdopener != -1)
		{
			close(cmdopener);
			tube = cmd_executor(cmd_path, args, envp, file, pids);
			free(cmd_path);
			break ;
		}
		free(cmd_path);
		i++;
	}
	return (tube);
}

int	totalcmds(char *cmd)
{
	int	i;
	int	total;

	i = 0;
	total = 1;
	while (cmd[i])
	{
		if (cmd[i] == '|')
			total++;
		i++;
	}
	printf("TOTAL = %d\n", total);
	return (total);
}

int	hasinfile(struct s_main *main)
{
	t_cmd_info	*actual_cmd;
	t_redir		*actual_redir;
	int			total;

	total = 0;
	actual_cmd = main->cmd_info;
	printf("AV\n");
	while (actual_cmd != NULL)
	{
		actual_redir = actual_cmd->redirs;
		while (actual_redir != NULL)
		{
			actual_redir->fd = open(actual_redir->filename, O_RDWR);
			printf(">>>\n");
			printf("FILE = %s\n", actual_redir->filename);
			if (actual_redir->fd == -1)
			{
				printf("Erreur de permissions lors de l'ouverture de %s\n", actual_redir->filename);
			}
			if (actual_redir->type == 3)
				total = 1;
			actual_redir = actual_redir->next;
		}
		actual_cmd = actual_cmd->next;
	}
	printf("AP\n");
	return (total);
}

int	hasoutfile(struct s_main *main)
{
	t_redir	*actual;
	int		total;

	total = 0;
	actual = main->cmd_info->redirs;
	while (actual != NULL)
	{
		printf("Enter\n");
		if (actual->type == 5)
			total = 1;
		actual = actual->next;			
	}
	return (total);
}

int	executor(char *cmd, struct s_main *main)
{
	pid_t		*pids;
	int			nbcmds;
	int			tube;
	char		**envp;
	int			i;

	envp = envp_to_str(main->datas);
	nbcmds = totalcmds(cmd);
	pids = malloc((totalcmds(cmd) + 1) * sizeof(pid_t));
	pids[0] = 0;
	tube = -1;
	hasinfile(main);
	if (nbcmds == 1)
	{
		setup_cmd_redirs(main->cmd_info);
		printf("EEE = %d\n", main->cmd_info->infile->fd);
		lcmd_searcher(main, envp, main->cmd_info->infile->fd, &pids);
	}
	else
	{
		while (nbcmds > 1)
		{
			setup_cmd_redirs(main->cmd_info);
			if (main->cmd_info->infile->fd != -1 && main->cmd_info->outfile->fd == -1)
			{
				printf("1 0 %s %d\n", main->cmd_info->cmd, main->cmd_info->infile->fd);
				tube = cmd_searcher(main, envp, main->cmd_info->infile->fd, main->cmd_info->argv, &pids);
				printf("ENDED\n");
			}
			else if (main->cmd_info->infile->fd == -1 && main->cmd_info->outfile->fd == -1)
			{
				printf("0 0\n");
				tube = cmd_searcher(main, envp, tube, main->cmd_info->argv, &pids);
			}
			else if (main->cmd_info->infile->fd == -1 && main->cmd_info->outfile->fd != -1)
			{
				printf("0 1\n");
				lcmd_searcher(main, envp, tube, &pids);
				tube = -1;
			}
			else if (main->cmd_info->infile->fd != -1 && main->cmd_info->outfile->fd != -1)
			{
				printf("1 1\n");
				lcmd_searcher(main, envp, main->cmd_info->infile->fd, &pids);
				tube = -1;
			}
			nbcmds--;
			main->cmd_info = main->cmd_info->next;
		}
		setup_cmd_redirs(main->cmd_info);
		if (main->cmd_info == NULL)
			printf("MMMOPMONO\n");
		if (nbcmds == 1 && /*tube != -1*/ main->cmd_info->infile->fd == -1)
		{
			printf("LOL2\n");
			lcmd_searcher(main, envp, tube, &pids);
		}
		else if (nbcmds == 1 && main->cmd_info->infile->fd != -1)
		{
			printf("LOL3 %d\n", main->cmd_info->infile->fd);
			lcmd_searcher(main, envp, main->cmd_info->infile->fd, &pids); 
		}
	}
	end_pids(&pids);
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	free(main->cmd_info);
	return (1);
}
