/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:51:55 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/09 16:52:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*create_redir(char *filename, char *content, t_token_type type)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	redir->filename = filename;
	redir->content = content;
	redir->type = type;
	redir->fd = -1;
	return (redir);
}

void	append_redir(t_redir **head, t_redir *new)
{
	t_redir	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	setup_fd(t_cmd_info **cmd_info)
{
	t_cmd_info	*actual;

	actual = *cmd_info;
	while (actual->redirs != NULL)
	{
		if (actual->redirs->type == 3 && actual->redirs->good == 1)
			actual->infile_fd = actual->redirs;
		if (actual->redirs->type == 3 && actual->redirs->good == 1)
			actual->infile_fd = actual->redirs;
		actual->redirs = actual->redirs->next;
	}
}