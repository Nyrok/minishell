/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:51:55 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/09 16:52:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd_info	*parse_redir(t_token *tokens, t_cmd_info **l, t_cmd_info *o)
{
	t_redir	*redir;

	if (!o)
	{
		o = create_cmd_info(NULL, 0);
		if (!*l)
			*l = o;
		else
			append_cmd_info(l, o);
	}
	redir = NULL;
	if (tokens->type == REDIN || tokens->type == HEREDOC)
		redir = create_redir(tokens->next->word, tokens->type);
	else if (tokens->type == REDOUT || tokens->type == APPEND)
		redir = create_redir(tokens->next->word, tokens->type);
	append_redir(&o->redirs, redir);
	return (o);
}

static void	parse_word(t_token *tokens, t_cmd_info **list, t_cmd_info **obj)
{
	if (!*obj)
	{
		*obj = create_cmd_info(tokens->word, count_cmd_args(tokens->next));
		if (!*list)
			*list = *obj;
		else
			append_cmd_info(list, *obj);
	}
	else if (!(*obj)->cmd)
	{
		(*obj)->cmd = ft_strdup(tokens->word);
		(*obj)->argc++;
		(*obj)->argv = ft_calloc(count_cmd_args(tokens->next) + 1, \
			sizeof(char *));
		if (!(*obj)->argv)
			return ;
		(*obj)->argv[0] = ft_strdup(tokens->word);
	}
	else
		(*obj)->argv[(*obj)->argc++] = ft_strdup(tokens->word);
}

t_cmd_info	*parse_tokens(t_main *main, t_token *tokens)
{
	t_cmd_info	*cmd_infos;
	t_cmd_info	*cmd_info;

	if (!check_tokens(tokens))
		return (NULL);
	parse_env(main->envp, tokens);
	cmd_infos = NULL;
	cmd_info = NULL;
	while (tokens)
	{
		if (tokens->type == WORD)
			parse_word(tokens, &cmd_infos, &cmd_info);
		else if (tokens->type == PIPE || tokens->type == END)
			cmd_info = NULL;
		else
		{
			cmd_info = parse_redir(tokens, &cmd_infos, cmd_info);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (cmd_infos);
}
