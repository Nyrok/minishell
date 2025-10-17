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

t_redir	*create_redir(char *filename, t_token_type type)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	redir->filename = ft_strdup(filename);
	redir->type = type;
	redir->fd = -1;
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
	t_redir		*actual_redir;

	actual_redir = cmd_info->redirs;
	while (actual_redir)
	{
		if (actual_redir->good)
		{
			if (actual_redir->io == STDIN_FILENO)
				cmd_info->infile = actual_redir;
			else if (actual_redir->io == STDOUT_FILENO)
				cmd_info->outfile = actual_redir;
		}
		actual_redir = actual_redir->next;
	}
}

t_token	*get_redir_token(const char *str, size_t *i)
{
	t_token	*token;

	token = NULL;
	if (str[*i] == '>' && ++(*i))
	{
		if (str[*i] == '>' && ++(*i))
			token = create_token(ft_strdup(">>"), APPEND);
		else
			token = create_token(ft_strdup(">"), REDOUT);
	}
	else if (str[*i] == '<' && ++(*i))
	{
		if (str[*i] == '<' && ++(*i))
			token = create_token(ft_strdup("<<"), HEREDOC);
		else
			token = create_token(ft_strdup("<"), REDIN);
	}
	return (token);
}
