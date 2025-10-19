/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fdcls(t_main **main, int error)
{
	t_cmd_info	*cmd_tmp;
	t_cmd_info	*to_free;
	t_redir		*actual_redir;

	cmd_tmp = (*main)->cmd_info;
	while (cmd_tmp)
	{
		to_free = cmd_tmp;
		actual_redir = to_free->redirs;
		while (actual_redir != NULL)
		{
			if (actual_redir->good != 1 && actual_redir->type == HEREDOC
				&& actual_redir->fd != -1)
				close_redir_fd(actual_redir);
			else if (actual_redir->type != HEREDOC && actual_redir->fd != -1)
				close_redir_fd(actual_redir);
			actual_redir = actual_redir->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	if (error == 1)
		(*main)->cmd_info = NULL;
	return (1);
}

void	append_opener(t_redir *actual_redir)
{
	if (actual_redir->io == STDOUT_FILENO \
		&& access(actual_redir->filename, F_OK) != 0)
	{
		actual_redir->fd = open(actual_redir->filename, O_CREAT, 0777);
		if (actual_redir->fd != -1)
			close(actual_redir->fd);
	}
}

void	end_fd(int fd)
{
	if (fd != -1)
	{
		close(fd);
		fd = -1;
	}
}
