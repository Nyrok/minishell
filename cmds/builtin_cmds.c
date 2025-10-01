/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup2_builtin(t_main *main, int nbcmds)
{
	if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
	{
		dup2(main->cmd_info->outfile->fd, STDOUT_FILENO);
		close(main->cmd_info->outfile->fd);
		if (nbcmds > 1)
		{
			close(main->cmd_info->tube[1]);
			main->cmd_info->tube[1] = -1;
		}
	}
	else if (nbcmds > 1)
	{
		dup2(main->cmd_info->tube[1], STDOUT_FILENO);
		close(main->cmd_info->tube[1]);
	}
}

int	builtin_exec_fork(t_main *main, t_envp **envp, int nbcmds, int onlyonecmd)
{
	dup2_builtin(main, nbcmds);
	if (ft_strcmp(main->cmd_info->cmd, "cd") == 0)
		cd(main, main->cmd_info->argc, main->cmd_info->argv[1], onlyonecmd);
	else if (ft_strcmp(main->cmd_info->cmd, "echo") == 0)
		echo(main, main->cmd_info->argc, (const char **)main->cmd_info->argv,
			nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "env") == 0)
		env(main, *envp, onlyonecmd, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "export") == 0)
		export(main, main->cmd_info->argc, main->cmd_info->argv, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "unset") == 0)
		unset(main, main->cmd_info->argc, envp);
	else if (ft_strcmp(main->cmd_info->cmd, "pwd") == 0)
		pwd(main, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "history") == 0 \
		&& main->cmd_info->argc >= 2 \
		&& ft_strncmp(main->cmd_info->argv[1], "-c", 2) == 0)
		list_history_cleaner(main);
	else if (ft_strcmp(main->cmd_info->cmd, "history") == 0)
		print_history(main->history);
	else if (ft_strcmp(main->cmd_info->cmd, "exit") == 0)
		ft_exit(&main, nbcmds, onlyonecmd);
	auto int exit_code = main->last_exit_status;
	close(main->cmd_info->tube[0]);
	free_execve(&main);
	exit(exit_code);
}

int	builtin_exec_no_fork(t_main *main, t_envp **envp, int nbcmds, int onlyonecmd)
{
	delete_tube(main);
	if (ft_strcmp(main->cmd_info->cmd, "cd") == 0)
		cd(main, main->cmd_info->argc, main->cmd_info->argv[1], onlyonecmd);
	else if (ft_strcmp(main->cmd_info->cmd, "echo") == 0)
		echo(main, main->cmd_info->argc, (const char **)main->cmd_info->argv,
			nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "env") == 0)
		env(main, *envp, onlyonecmd, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "export") == 0)
		export(main, main->cmd_info->argc, main->cmd_info->argv, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "unset") == 0)
		unset(main, main->cmd_info->argc, envp);
	else if (ft_strcmp(main->cmd_info->cmd, "pwd") == 0)
		pwd(main, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "history") == 0 \
		&& main->cmd_info->argc >= 2 \
		&& ft_strncmp(main->cmd_info->argv[1], "-c", 2) == 0)
		list_history_cleaner(main);
	else if (ft_strcmp(main->cmd_info->cmd, "history") == 0)
		print_history(main->history);
	else if (ft_strcmp(main->cmd_info->cmd, "exit") == 0)
		ft_exit(&main, nbcmds, onlyonecmd);
	else
		return (0);
	return (1);
}

int	isbuilt_in(t_main *main)
{
	if (ft_strcmp(main->cmd_info->cmd, "cd") == 0 ||
		ft_strcmp(main->cmd_info->cmd, "echo") == 0
		|| ft_strcmp(main->cmd_info->cmd, "env") == 0
		|| ft_strcmp(main->cmd_info->cmd, "export") == 0
		|| ft_strcmp(main->cmd_info->cmd, "unset") == 0
		|| ft_strcmp(main->cmd_info->cmd, "pwd") == 0
		|| ft_strcmp(main->cmd_info->cmd, "history") == 0
		|| ft_strcmp(main->cmd_info->cmd, "history") == 0
		|| ft_strcmp(main->cmd_info->cmd, "exit") == 0)
	{
		return (1);
	}
	return (0);
}

int	builtin_exec(t_main *main, t_envp **envp, int nbcmds, int onlyonecmd)
{
	pid_t	pid;
	int		return_value;

	return_value = 0;
	if (onlyonecmd == 1 && isbuilt_in(main) == 1)
		return_value = builtin_exec_no_fork(main, envp, nbcmds, onlyonecmd);
	else if (isbuilt_in(main) == 1 && nbcmds == 1)
	{
		delete_tube(main);
		pid = fork();
		if (pid == 0)
			return_value = builtin_exec_fork(main, envp, nbcmds, onlyonecmd);
		else
		{
			add_pid(main, pid);
			return_value = 1;
		}
	}
	else if (isbuilt_in(main) == 1 && nbcmds > 1)
	{
		if (pipe(main->cmd_info->tube) == -1)
		{
			perror("pipe");
			return (-1);
		}
		pid = fork();
		if (pid == 0)
			return_value = builtin_exec_fork(main, envp, nbcmds, onlyonecmd);
		else
		{
			close(main->cmd_info->tube[1]);
			if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
				close(main->cmd_info->outfile->fd);
			if (nbcmds > 1 && main->tube && main->tube->fd != -1)
				close(main->tube->fd);
			if (main->tube)
				main->tube->fd = main->cmd_info->tube[0];
			add_pid(main, pid);
			return_value = 1;
		}
	}
	printf("CC = %d\n", return_value);
	return (return_value);
}
