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

static t_token *parse_other(t_token *tokens)
{
	if (tokens->type == REDIN)
	{
		// parse_data->infile = tokens->next->word;
		tokens = tokens->next;
	}
	else if (tokens->type == REDOUT || tokens->type == APPEND)
	{
		// parse_data->outfile = tokens->next->word;
		tokens = tokens->next;
	}
	return (tokens);
}

static void parse_word(t_token *tokens, t_cmd_info *cmd_infos, t_cmd_info *cmd_info)
{
	if (!cmd_info)
	{
		cmd_info = create_cmd_info(NULL, tokens->word, NULL, count_cmd_args(tokens->next));
		append_cmd_info(&cmd_infos, cmd_info);
	}
	else
		cmd_info->argv[cmd_info->argc++] = tokens->word;
}

t_cmd_info *parse_tokens(t_token *tokens)
{
	t_cmd_info *cmd_infos;
	t_cmd_info *cmd_info;

	if (!check_tokens(tokens))
		return (NULL);
	cmd_infos = NULL;
	cmd_info = NULL;
	while (tokens)
	{
		if (tokens->type != WORD)
		{
			cmd_info = NULL;
			tokens = parse_other(tokens);
		}
		else if (tokens->type == WORD)
			parse_word(tokens, cmd_infos, cmd_info);
		tokens = tokens->next;
	}
	return (cmd_infos);
}
