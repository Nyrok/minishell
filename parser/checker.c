/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:51:55 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/09 16:52:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_tokens(t_token *tokens)
{
	t_token *token;

	token = tokens;
	while (token)
	{
		if (token->type != WORD && token->type != END)
		{
			if (!token->next || token->next->type != WORD)
			{
				token->next->type = INVALID;
				printf("Unexpected token '%s' after '%s'\n", token->next->word, token->word);
				return (0);
			}
		}
		token = token->next;
	}
	return (1);
}
