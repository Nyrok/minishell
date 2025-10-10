/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_outfile(t_main *main)
{
	if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
	{
		close(main->cmd_info->outfile->fd);
		main->cmd_info->outfile->fd = -1;
	}
}

void	add_pid(t_main *main, pid_t newpid)
{
	int		i;
	pid_t	*actual;

	i = 0;
	actual = main->pids;
	while (actual[i] != 0)
		i++;
	actual[i] = newpid;
	actual[i + 1] = 0;
}

void	end_pids(t_main **main)
{
	int		i;
	int		status;
	pid_t	*actual;

	i = 0;
	actual = (*main)->pids;
	while (actual[i] != 0)
	{
		waitpid(actual[i], &status, 0);
		i++;
	}
	if (i > 0)
	{
		if (WIFEXITED(status))
			(*main)->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			(*main)->last_exit_status = 128 + WTERMSIG(status);
	}
	i = 3;
	free(actual);
}

void	ft_strdup_cmd_path(t_main *main, int i)
{
	if (!main->cmds_paths->paths[i] && main->cmd_info \
		&& ft_strlen(main->cmd_info->cmd) == 0)
	{
		free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
		main->last_exit_status = 127;
	}
}

int	create_eof_fd(t_main *main)
{
	int	pipefd[2];

	if (main->tube && main->tube->fd != -1)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
