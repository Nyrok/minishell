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

int	check_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type != WORD && tokens->type != END)
		{
			if (tokens->type == PIPE && tokens->next)
			{
				if (tokens->next->type != END && tokens->next->type != PIPE)
				{
					tokens = tokens->next;
					continue ;
				}
			}
			if (!tokens->next || tokens->next->type != WORD)
			{
				tokens->next->type = INVALID;
				printf("Unexpected tokens '%s' after '%s'\n", \
					tokens->next->word, tokens->word);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}
