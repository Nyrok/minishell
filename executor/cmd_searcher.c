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

char	**envp_str(t_envp *datas)
{
	char	**res;
	t_envp	*actual;
	int		i;

	i = 0;
	actual = datas;
	while (actual != NULL)
	{
		i++;
		actual = actual->next;
	}
	res = malloc((i + 1) * sizeof(char *));
	if (res == NULL)
		return (0);
	actual = datas;
	i = 0;
	while (actual != NULL)
	{
		res[i] = actual->data;
		actual = actual->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}

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
		if (tube != NULL)
			dup2(tube, STDIN_FILENO);
		if (file != NULL)
			dup2(file, STDOUT_FILENO);
		close(file);
		close(tube);
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
		close(tube);
		close(file);
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
	args = ft_split(cmd, ' ');
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
		if (file != NULL)
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

int	executor(char *cmd, char **paths, char **envp)
{
	pid_t		pids;
	int			nbcmds;
	info_cmd_t	cmd_info;
	int			tube;

	nbcmds = totalcmds(...);
	if (nbcmds == 1)
		lcmd_searcher(...); // Si il n'y a pas de outfile on met NULL;
	else
	{
		while (nbcmds > 0)
		{
			cmd_info = get_cmd(...); // Si il n'y a pas de infile on met NULL;
			if (cmd_info.infile != NULL)
				tube = cmd_searcher(...); // Le tube passera toujours en paramètre sauf en cas de infile qui du coup sera prioritaire
			else if (cmd_info.outfile == NULL)
				tube = cmd_searcher(...); // Pas de infile donc on prend le tube en param
			else if (cmd_info.outfile != NULL)
				lcmd_searcher(...); // Si on fait ls | cat -e > outfile.txt | cat -e > outfile2.txt par exemple, le premier cat -e enverra les données vers outfile donc le second sera vide, il faut donc prendre cela en compte et cette fonction gère cela
			nbcmds--;
		}
		if (nbcmds == 1)
			lcmd_searcher(...); // Si il n'y a pas de outfile on met NULL;
	}
	return (1);
}