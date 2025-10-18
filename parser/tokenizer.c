/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:51:55 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/09 16:52:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_token(t_token **head, t_token *new)
{
	t_token	*tmp;

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

t_token	*create_token(char *word, t_token_type type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->word = word;
	token->type = type;
	return (token);
}

t_token	*tokenize_input(const char *input)
{
	size_t	i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	if (!input)
		return (NULL);
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (input[i] == '\'' || input[i] == '"')
			append_token(&tokens, create_token(get_word(input, &i), WORD));
		else if (input[i] == '|' && ++i)
			append_token(&tokens, create_token(ft_strdup("|"), PIPE));
		else if (input[i] == '>' || input[i] == '<')
			append_token(&tokens, get_redir_token(input, &i));
		else if (input[i] != '\0')
			append_token(&tokens, create_token(get_word(input, &i), WORD));
	}
	append_token(&tokens, create_token(NULL, END));
	return (tokens);
}
