/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_executor(t_main *main, int file, int last, int onlyonecommand)
{
	char	**tmp;
	int		i;

	i = 0;
	isfilevalid(main);
	tmp = ft_split(main->cmd_info->cmd, ' ');
	main->cmd_info->cmd_path = ft_strdup(tmp[0]);
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
	if (last == 0)
		main->tube->fd = cmd_executor(main, main->str_envp, file, -1);
	else
		last_executor(main, main->str_envp, main->tube->fd, onlyonecommand);
	free(main->cmd_info->cmd_path);
	main->cmd_info->cmd_path = NULL;
	return (1);
}

void	ft_dup2(int src, int dest)
{
	if (dup2(src, dest) == -1)
		perror("dup2 failed");
}

void	child_executor(t_main *main, int *tube, int file, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	close(tube[0]);
	if (file != -1)
	{
		ft_dup2(file, STDIN_FILENO);
		close(file);
		file = -1;
	}
	if (tube[1] != -1)
	{
		ft_dup2(tube[1], STDOUT_FILENO);
		close(tube[1]);
		tube[1] = -1;
	}
	if (execve(main->cmd_info->cmd_path,
			(char *const *)main->cmd_info->argv, envp) == -1)
	{
		if (tube[1] != -1)
			close(tube[1]);
		if (file != -1)
			close(file);
		auto int exit_code = free_execve(&main);
		perror("execve failed");
		exit(exit_code);
	}
}

int	cmd_executor(t_main *main, char **envp, int file, int i)
{
	pid_t	pid;
	int		tube[2];

	if (pipe(tube) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (i != -1 && main->cmds_paths->paths[i] == NULL)
	{
		close(tube[1]);
		tube[1] = -1;
	}
	pid = fork();
	if (pid == 0)
		child_executor(main, tube, file, envp);
	else
	{
		if (tube[1] != -1)
			close(tube[1]);
		if (file != -1)
			close(file);
		add_pid(main, pid);
	}
	return (tube[0]);
}

int	executor(char *cmd, struct s_main *main)
{
	int	nbcmds;

	main->pids = malloc((totalcmds(cmd) + 1) * sizeof(pid_t));
	if (!main->pids)
		return (0);
	main->str_envp = envp_to_str(main->envp);
	if (executor_setup(&main, &nbcmds, cmd) == -1)
		return (free_cmd_info(&main->cmd_info), no_leaks(main),
			end_pids(&main), 0);
	if (nbcmds == 1)
	{
		if (onecmdexector(main, main->str_envp) == -1)
			return (-1);
		free_cmd_info(&main->cmd_info);
	}
	else
	{
		if (multiple_cmd_handler(main, main->str_envp, nbcmds, 0) == -1)
			return (-1);
	}
	if (main->pids)
		end_pids(&main);
	no_leaks(main);
	return (1);
}
