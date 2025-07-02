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
			tmp = tmp->next;
		tmp->next = new;
		if (new && tmp->type == new->type)
			tmp->good = 0;
	}
}
