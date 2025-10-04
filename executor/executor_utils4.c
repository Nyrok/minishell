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

int	ft_access(t_main *main, char *pathname)
{
	if (access(pathname, X_OK) != 0)
	{
		printf("-minishell: %s: Permission denied", pathname);
		main->last_exit_status = 127;
		return (0);
	}
	return (1);
}

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
