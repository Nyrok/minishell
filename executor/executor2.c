/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                         :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	last_child_executor(int tube, t_main *main, char *cmd_path, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	if (tube != -1)
	{
		dup2(tube, STDIN_FILENO);
		close(tube);
		tube = -1;
	}
	if (main->tube && main->tube->fd != -1)
		close(main->tube->fd);
	if (main->cmd_info->outfile != NULL)
	{
		fd_opener(&main, main->cmd_info->outfile, 0);
		if (dup2(main->cmd_info->outfile->fd, STDOUT_FILENO) == -1)
			perror("dup2 failed");
		close(main->cmd_info->outfile->fd);
	}
	if (execve(cmd_path, (char *const *)main->cmd_info->argv, envp) == -1)
	{
		if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
			close(main->cmd_info->outfile->fd);
		free_execve(&main);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	last_executor(t_main *main, char **envp, int tube)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		last_child_executor(tube, main, main->cmd_info->cmd_path, envp);
	}
	else
	{
		if (tube != -1)
			close(tube);
		if (main->cmd_info->outfile != NULL)
			close(main->cmd_info->outfile->fd);
		add_pid(main, pid);
	}
}

int	executor_setup(t_main **main, int *nbcmds, char *cmd)
{
	*nbcmds = totalcmds(cmd);
	(*main)->pids[0] = 0;
	setup_tube(*main);
	return (1);
}

int	onecmdexector(t_main *main, char **envp)
{
	if (main->cmd_info->cmd == NULL)
		return (handle_heredoc(main), create_out(main), end_pids(&main),
			free_all_cmd_info(&main), no_leaks(main), -1);
	setup_cmd_redirs(main->cmd_info);
	if (hasinfile(&main, 0) == -1)
	{
		return (free_cmd_info(&main->cmd_info),
			no_leaks(main), 0); // end_pids(&main, pids) retiré
	}
	if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
		close(main->cmd_info->outfile->fd);
	if (builtin_exec(main, &main->envp, 1, 1) == 1)
		return (1);
	if (main->cmd_info->infile != NULL)
		lcmd_searcher(main, envp, main->cmd_info->infile->fd);
	else
		lcmd_searcher(main, envp, -1);
	return (1);
}

<<<<<<< HEAD
int	multiplecmdexector(t_main *main, char **envp, int nbcmds)
{
	if (tube_handler(&main) == -1)
		return (-1);
	if (builtin_exec(main, &main->envp, nbcmds, 0) == 1)
		return (1);
	if (main->cmd_info->infile != NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
	{
		main->tube->fd = cmd_searcher(main, envp,
				main->cmd_info->infile->fd);
	}
	else if (main->cmd_info->infile == NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
		main->tube->fd = cmd_searcher(main, envp,
				main->tube->fd);
	else if (main->cmd_info->infile == NULL)
	{
		lcmd_searcher(main, envp, main->tube->fd);
		main->tube->fd = -1;
	}
	else if (main->cmd_info->infile != NULL)
	{
		lcmd_searcher(main, envp, main->cmd_info->infile->fd);
		main->tube->fd = -1;
	}
	return (1);
}
