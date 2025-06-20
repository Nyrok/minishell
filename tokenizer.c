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

static t_token	*create_token(char *word, t_token_type type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->word = word;
	token->type = type;
	return (token);
}

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

static char	*get_word(const char *str, size_t *i)
{
	size_t	start;
	size_t	quotes_count;
	int		has_quote;

	start = *i;
	has_quote = 0;
	quotes_count = 0;
	while (str[*i] && !ft_strchr("|<>", str[*i]) \
		&& (!ft_isspace(str[*i]) || has_quote))
	{
		if ((str[*i] == '\'' || str[*i] == '"') && ++quotes_count)
			has_quote = !has_quote;
		(*i)++;
	}
	return (rm_quotes(str, start, *i - start));
}

static char	*get_quoted(const char *str, size_t *i)
{
	char	quote;
	size_t	start;
	char	*word;

	quote = str[*i];
	start = ++(*i);
	while (str[*i] && str[*i] != quote)
		(*i)++;
	word = ft_substr(str, start, *i - start);
	if (str[*i] == quote)
		(*i)++;
	return (word);
}

t_token	*tokenize_input(const char *input)
{
	size_t	i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (input[i] == '\'' || input[i] == '"')
			append_token(&tokens, create_token(get_quoted(input, &i), WORD));
		else if (input[i] == '|' && ++i)
			append_token(&tokens, create_token(ft_strdup("|"), PIPE));
		else if (input[i] == '>' && ++i)
			append_token(&tokens, create_token(ft_strdup(">"), REDOUT + \
			(input[i] == '>' && ++i)));
		else if (input[i] == '<' && ++i)
			append_token(&tokens, create_token(ft_strdup("<"), REDIN + \
			(input[i] == '<' && ++i)));
		else
			append_token(&tokens, create_token(get_word(input, &i), WORD));
		i++;
	}
	return (tokens);
}
