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

int	no_such_file(t_redir *actual_redir, int print)
{
	if (print == 1)
		printf("-minishell: %s: No such file or directory\n",
			actual_redir->filename);
	return (0);
}

int	permission_denied(t_main **main, t_redir *actual_redir, int print)
{
	if (actual_redir->type == REDOUT
		&& access(actual_redir->filename, F_OK) != 0)
	{
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
		return (0);
	}
	if (print == 1)
		printf("-minishell: %s: Permission denied\n", actual_redir->filename);
	(*main)->last_exit_status = 126;
	if ((*main)->cmd_info && (*main)->cmd_info->cmd
		&& ft_strcmp((*main)->cmd_info->cmd, "echo") == 0)
		(*main)->last_exit_status = 1;
	return (0);
}
