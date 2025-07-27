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
	if (tube != -1)
	{
		dup2(tube, STDIN_FILENO);
		close(tube);
	}
	if (main->cmd_info->outfile != NULL)
	{
		dup2(main->cmd_info->outfile->fd, STDOUT_FILENO);
		close(main->cmd_info->outfile->fd);
	}
	if (execve(cmd_path, (char *const *)main->cmd_info->argv, envp) == -1)
	{
		close(main->cmd_info->outfile->fd);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	last_executor(t_main *main, char **envp, int tube, pid_t **pids)
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
		add_pid(pids, pid);
	}
}

int	executor_setup(t_main *main, pid_t *pids, int *nbcmds, char *cmd)
{
	*nbcmds = totalcmds(cmd);
	pids[0] = 0;
	hasinfile(main);
	setup_tube(main);
	return (1);
}

int	onecmdexector(t_main *main, char **envp, pid_t **pids)
{
	setup_cmd_redirs(main->cmd_info);
	if (builtin_exec(main, pids, &main->envp, 1) == 1)
		return (1);
	if (main->cmd_info->infile != NULL)
		lcmd_searcher(main, envp, main->cmd_info->infile->fd, pids);
	else
		lcmd_searcher(main, envp, -1, pids);
	return (1);
}

int	multiplecmdexector(t_main *main, char **envp, pid_t **pids, int nbcmds)
{
	if (builtin_exec(main, pids, &main->envp, nbcmds) == 1)
		return (1);
	if (main->cmd_info->infile != NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
	{
		main->tube->fd = cmd_searcher(main, envp,
				main->cmd_info->infile->fd, pids);
	}
	else if (main->cmd_info->infile == NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
	{
		main->tube->fd = cmd_searcher(main, envp,
				main->tube->fd, pids);
	}
	else if (main->cmd_info->infile == NULL)
	{
		lcmd_searcher(main, envp, main->tube->fd, pids);
		main->tube->fd = -1;
	}
	else if (main->cmd_info->infile != NULL)
	{
		lcmd_searcher(main, envp, main->cmd_info->infile->fd, pids);
		main->tube->fd = -1;
	}
	return (1);
}
