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

void	print_error(t_main *main, int error_code, int cmd_found)
{
	if (error_code == NOTFOUND && cmd_found == 0)
	{
		printf("%s: command not found\n", main->cmd_info->cmd);
		main->tube->fd = -1;
	}
}

int	cmd_executor(char *cmd_path, char **args, char **envp, int file, pid_t **pids);

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

void	last_child_executor(int tube, t_main *main, char *cmd_path, char **envp)
{
	if (tube != -1)
	{
		//printf("PRINTF %d, %s, %d\n", tube, main->cmd_info->infile->filename, main->cmd_info->infile->fd);
		dup2(tube, STDIN_FILENO);
		close(tube);
	}
	if (main->cmd_info->outfile != NULL)
	{
		printf("PRINTF2\n");
		dup2(main->cmd_info->outfile->fd, STDOUT_FILENO);
		printf("PRINTF3\n");
		close(main->cmd_info->outfile->fd);
	}
	if (execve(cmd_path, (char *const *)main->cmd_info->argv, envp) == -1)
	{
		close(main->cmd_info->outfile->fd);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
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
		last_child_executor(tube, main, cmd_path, envp);
		// while (main->cmd_info->argv[i])
		// 	free(main->cmd_info->argv[i++]);
		// free(main->cmd_info->argv);
	}
	else
	{
    	while (main->cmd_info->argv[i])
			free(main->cmd_info->argv[i++]);
		free(main->cmd_info->argv);
		if (tube != -1)
			close(tube);
		if (main->cmd_info->outfile != NULL)
			close(main->cmd_info->outfile->fd);
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

int	file_executor(t_main *main, char **envp, int file, pid_t **pids, int last)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = ft_split(main->cmd_info->cmd, ' ');
	main->cmd_info->cmd_path = ft_strdup(tmp[0]);
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
	if (last == 0)
		main->tube->fd = cmd_executor(main->cmd_info->cmd_path, main->cmd_info->argv, envp, file, pids);
	else
	{
		last_executor(main->cmd_info->cmd_path, main, envp, main->tube->fd, pids);
		main->tube->fd = -1;
	}
	free(main->cmd_info->cmd_path);
	return (1);
}

void	lcmd_searcher(t_main *main, char **envp, int tube, pid_t **pids)
{
	int		i;
	int		cmdopener;
	int		cmd_found;

	cmd_found = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
		cmd_found = file_executor(main, envp, -1, pids, 1);
	i = 0;
	while (main->cmds_paths->paths[i])
	{
		main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd, main->cmd_info->cmd_path, main->cmds_paths->paths[i]);
		cmdopener = open(main->cmd_info->cmd_path, O_RDONLY);
		if (cmdopener != -1)
		{
			close(cmdopener);
			cmd_found = 1;
			last_executor(main->cmd_info->cmd_path, main, envp, tube, pids);
			free(main->cmd_info->cmd_path);
			break ;
		}
		//close(cmdopener);
		free(main->cmd_info->cmd_path);
		i++;
	}
	print_error(main, NOTFOUND, cmd_found);
}

void	child_executor(int *tube, int file, char *cmd_path, char **args, char **envp)
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
	(void)args;
	// while (args[i])
	// 	free(args[i++]);
	// free(args);
}

int	cmd_executor(char *cmd_path, char **args, char **envp, int file, pid_t **pids)
{
	pid_t	pid;
	//int		status;
	int		tube[2];
	int	i;

	i = 0;
	if (pipe(tube) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
	{
		child_executor(tube, file, cmd_path, args, envp);
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
	int		cmdopener;
	int		cmd_found;

	cmd_found = 0;
	i = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
		cmd_found = file_executor(main, envp, file, pids, 0);
	while (main->cmds_paths->paths[i])
	{
		main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd, main->cmd_info->cmd_path, main->cmds_paths->paths[i]);
		cmdopener = open(main->cmd_info->cmd_path, O_RDONLY);
		if (cmdopener != -1)
		{
			close(cmdopener);
			cmd_found = 1;
			main->tube->fd = cmd_executor(main->cmd_info->cmd_path, args, envp, file, pids);
			free(main->cmd_info->cmd_path);
			break ;
		}
		free(main->cmd_info->cmd_path);
		i++;
	}
	print_error(main, NOTFOUND, cmd_found);
	return (main->tube->fd);
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

int ft_heredoc(char *end)
{
	char *line;
	int  fd[2];

	printf("entree\n");
	line = readline("> ");
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (line != NULL && ft_strcmp(line, end) != 0)
	{
		write(fd[1], &line, ft_strlen(line)); // check si res est define pour le tout premier join
		free(line);
		line = readline("> ");
	}
	free(line);
	return (fd[0]);
}

int	fd_opener(t_redir *actual_redir)
{
	if (actual_redir->type == APPEND)
		actual_redir->fd = open(actual_redir->filename, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (actual_redir->type == REDIN)
		actual_redir->fd = open(actual_redir->filename, O_RDONLY, 0444);
	else if (actual_redir->type == HEREDOC)
	{
		printf("entree\n");
		actual_redir->fd = ft_heredoc(actual_redir->filename);
	}
	else
		actual_redir->fd = open(actual_redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (actual_redir->fd == -1)
		return (0);
	if (actual_redir->good == 0)
		close(actual_redir->fd);
	return (1);
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
			if (fd_opener(actual_redir) == 0)
				return (0);
			printf(">>>\n");
			printf("FILE = %s\n", actual_redir->filename);
			if (actual_redir->fd == -1)
				printf("Erreur de permissions lors de l'ouverture de %s\n", actual_redir->filename);
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

void	setup_tube(t_main *main)
{
	main->tube = ft_calloc(1, sizeof(t_redir));
	main->tube->next = NULL;
	main->tube->fd = -1;
}

int	onecmdexector(t_main *main, char **envp, pid_t *pids)
{
	setup_cmd_redirs(main->cmd_info);
	if (builtin_exec(main, main->cmd_info, &main->datas, main->history, 1) == 1)
		return (1);
	if (main->cmd_info->infile != NULL)
		lcmd_searcher(main, envp, main->cmd_info->infile->fd, &pids);
	else
		lcmd_searcher(main, envp, -1, &pids);
	return (1);
}

int	multiplecmdexector(t_main *main, char **envp, pid_t *pids, int nbcmds)
{
	if (builtin_exec(main, main->cmd_info,
		&main->datas, main->history, nbcmds) == 1)
		return (1);
	if (main->cmd_info->infile != NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
	{
		printf("1 0 %s %d\n", main->cmd_info->cmd, main->cmd_info->infile->fd);
		main->tube->fd = cmd_searcher(main, envp, 
			main->cmd_info->infile->fd, main->cmd_info->argv, &pids);
		printf("89¨@#°2+1/ENDED\n");
	}
	else if (main->cmd_info->infile == NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
	{
		printf("0 0\n");
		main->tube->fd = cmd_searcher(main, envp, 
			main->tube->fd, main->cmd_info->argv, &pids);
	}
	else if (main->cmd_info->infile == NULL)
	{
		printf("0 1\n");
		lcmd_searcher(main, envp, main->tube->fd, &pids);
		main->tube->fd = -1;
	}
	else if (main->cmd_info->infile != NULL)
	{
		printf("1 1\n");
		lcmd_searcher(main, envp, main->cmd_info->infile->fd, &pids);
		main->tube->fd = -1;
	}
	return (1);
}

int	executor_setup(t_main *main, pid_t *pids, int *nbcmds, char *cmd)
{
	*nbcmds = totalcmds(cmd);
	pids[0] = 0;
	hasinfile(main);
	setup_tube(main);
	return (1);
}

int	no_leaks(t_main *main, char **envp)
{
	int	i;

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

int	executor(char *cmd, struct s_main *main)
{
	pid_t		*pids;
	int			nbcmds;
	char		**envp;

	pids = malloc((totalcmds(cmd) + 1) * sizeof(pid_t));
	if (!pids)
		return (0);
	envp = envp_to_str(main->datas);
	executor_setup(main, pids, &nbcmds, cmd);
	if (main->cmd_info->cmd == NULL)
	{
		return (0);
	}
	if (nbcmds == 1)
		onecmdexector(main, envp, pids);
	else
	{
		while (nbcmds > 0)
		{
			setup_cmd_redirs(main->cmd_info);
			multiplecmdexector(main, envp, pids, nbcmds--);
			main->cmd_info = main->cmd_info->next;
		}
	}
	end_pids(&pids);
	no_leaks(main, envp);
	return (1);
}
