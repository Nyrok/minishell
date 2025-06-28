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

char	**get_args(char *cmd);

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

void	last_executor(char *cmd_path, char **args, char **envp, int tube, int file, pid_t **pids)
{
	pid_t	pid;
	//int		status;
	int		i;

	i = 0;
	pid = fork();
	if (pid == 0)
	{
		while (args[i])
		{
			printf("ARG %s\n", args[i]);
			i++;
		}
		i = 0;
		printf("TEST %s\n", cmd_path);
		if (tube != 0)
		{
			dup2(tube, STDIN_FILENO);
			close(tube);
		}
		if (file != -1)
		{
			dup2(file, STDOUT_FILENO);
			close(file);
		}
		if (execve(cmd_path, (char *const *)args, envp) == -1)
		{
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
		// if (tube != 0)
		// {
		// 	dup2(tube, STDIN_FILENO);
		// 	close(tube);
		// }
		// if (file != -1)
		// {
		// 	dup2(file, STDOUT_FILENO);
		// 	close(file);
		// }
		add_pid(pids, pid);
		end_pids(pids);
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

void	lcmd_searcher(char *cmd, char **paths, char **envp, int tube, int file, pid_t  **pids)
{
	int		i;
	char	*cmd_path;
	char	**args;
	int		cmdopener;
	char	**tmp;

	i = 0;
	args = get_args(cmd);
	if (cmd[0] == '.' && cmd[1] == '/')
	{
		tmp = ft_split(cmd, ' ');
		cmd_path = ft_strdup(tmp[0]);
		while (tmp[i])
			free(tmp[i++]);
		free(tmp);
		last_executor(cmd_path, args, envp, tube, file, pids);
		free(cmd_path);
	}
	i = 0;
	while (paths[i])
	{
		cmd_path = paths_searcher(cmd, cmd_path, paths[i]);
		cmdopener = open(cmd_path, O_RDONLY);
		if (cmdopener != -1)
		{
			close(cmdopener);
			last_executor(cmd_path, args, envp, tube, file, pids);
			free(cmd_path);
			break ;
		}
		close(cmdopener);
		free(cmd_path);
		i++;
	}
}

int	cmd_executor(char *cmd_path, char **args, char **envp, int file, pid_t  **pids)
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

int	cmd_searcher(char *cmd, char **paths, char **envp, int file, char **args, pid_t **pids)
{
	int		i;
	char	*cmd_path;
	int		tube;
	int		cmdopener;
	char	**tmp;

	i = 0;
	if (cmd[0] == '.' && cmd[1] == '/')
	{
		tmp = ft_split(cmd, ' ');
		cmd_path = ft_strdup(tmp[0]);
		while (tmp[i])
			free(tmp[i++]);
		free(tmp);
		tube = cmd_executor(cmd_path, args, envp, file, pids);
		free(cmd_path);
	}
	i = 0;
	while (paths[i])
	{
		cmd_path = paths_searcher(cmd, cmd_path, paths[i]);
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

void	get_cmd(t_info_cmd **cmd_info, char *cmd)
{
	t_info_cmd	*actual;

	(void)cmd;
	actual = *cmd_info;
	actual->infile = ft_strdup("infile.txt");
	actual->fd_infile = open("infile.txt", O_RDWR);
	actual->outfile = ft_strdup("outfile.txt");
	actual->fd_outfile = open("outfile.txt", O_RDWR);
	actual->next = NULL;
}

char	**get_args(char *cmd)
{
	char	**args;

	(void)cmd;
	args = malloc(3 * sizeof(char *));
	args[0] = ft_strdup("ls");
	args[1] = ft_strdup("-a");
	args[2] = NULL;
	return (args);
}

int	executor(char *cmd, char **paths, t_envp *envp_list)
{
	pid_t		*pids;
	int			nbcmds;
	t_info_cmd	*cmd_info;
	int			tube;
	char		**envp;
	int			i;

	cmd_info = malloc(sizeof(t_info_cmd));
	envp = envp_to_str(envp_list);
	get_cmd(&cmd_info, cmd);
	nbcmds = totalcmds(cmd);
	pids = malloc((totalcmds(cmd) + 1) * sizeof(pid_t));
	pids[0] = 0;
	if (nbcmds == 1)
	{
		printf("CAS 1, %s %s %d\n", cmd_info->infile, cmd_info->outfile, cmd_info->fd_outfile);
		lcmd_searcher(cmd, paths, envp, cmd_info->fd_infile, cmd_info->fd_outfile, &pids); // Si il n'y a pas de outfile on met NULL;
	}
	else
	{
		while (nbcmds > 0)
		{
			cmd_info->args = get_args(cmd);
			// cmd_info = get_cmd(...); // Si il n'y a pas de infile on met NULL;
			if (cmd_info->infile != NULL && cmd_info->outfile == NULL)
				tube = cmd_searcher(cmd, paths, envp, cmd_info->fd_infile, cmd_info->args, &pids); // Le tube passera toujours en paramètre sauf en cas de infile qui du coup sera prioritaire
			else if (cmd_info->infile == NULL && cmd_info->outfile == NULL)
				tube = cmd_searcher(cmd, paths, envp, cmd_info->fd_infile, cmd_info->args, &pids); // Pas de infile donc on prend le tube en param
			else if (cmd_info->infile == NULL && cmd_info->outfile != NULL)
				lcmd_searcher(cmd, paths, envp, tube, cmd_info->fd_outfile, &pids); // Si on fait ls | cat -e > outfile.txt | cat -e > outfile2.txt par exemple, le premier cat -e enverra les données vers outfile donc le second sera vide, il faut donc prendre cela en compte et cette fonction gère cela
			else if (cmd_info->infile != NULL && cmd_info->outfile != NULL)
				lcmd_searcher(cmd, paths, envp, cmd_info->fd_infile, cmd_info->fd_outfile, &pids); // mettre infile au lieu de tube
			nbcmds--;
			cmd_info = cmd_info->next; // Faire un temp pour free cmd_info
		}
		if (nbcmds == 1)
			lcmd_searcher(cmd, paths, envp, cmd_info->fd_infile, cmd_info->fd_outfile, &pids); // Si il n'y a pas de outfile on met NULL;
	}
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	free(cmd_info);
	return (1);
}
*/