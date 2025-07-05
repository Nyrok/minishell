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

t_redir	*create_redir(char *filename, char *content, t_token_type type, int io)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	redir->filename = filename;
	redir->content = content;
	redir->type = type;
	redir->io = -1;
	if (type == REDIN || type == HEREDOC)
		redir->io = STDIN_FILENO;
	else if (type == REDOUT || type == APPEND)
		redir->io = STDOUT_FILENO;
	redir->good = 1;
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
		{
			if (new && tmp->io == new->io)
				tmp->good = 0;
			tmp = tmp->next;
		}
		tmp->next = new;
		if (new && tmp->io == new->io)
			tmp->good = 0;
	}
}

void	setup_cmd_redirs(t_cmd_info *cmd_info)
{
	t_cmd_info	*actual;

	actual = cmd_info;
	while (actual->redirs != NULL)
	{
		if (actual->redirs->good)
		{
			if (actual->redirs->io == STDIN_FILENO)
				actual->infile_redir = actual->redirs;
			else if (actual->redirs->io == STDOUT_FILENO)
				actual->outfile_redir = actual->redirs;
		}
		actual->redirs = actual->redirs->next;
	}
}
