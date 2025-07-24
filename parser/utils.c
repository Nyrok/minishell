/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:51:55 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/09 16:52:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rm_quotes(char *str, size_t start, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	size;
	int		quotes;
	char	*result;

	quotes = 0;
	i = -1;
	while (++i < n)
		if (str[start + i] == '\'' || str[start + i] == '"')
			quotes++;
	size = n - quotes;
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < n)
		if (str[start + i] != '\'' && str[start + i] != '"')
			result[j++] = str[start + i];
	result[i] = '\0';
	return (result);
}

char	*get_word(const char *str, size_t *i)
{
	size_t	start;
	int		has_quote;
	char	quote;

	start = *i;
	has_quote = 0;
	while (str[*i])
	{
		if (!has_quote && (ft_isspace(str[*i]) || ft_strchr("|<>", str[*i])) \
		&& (*i)--)
			break ;
		if ((str[*i] == '\'' || str[*i] == '"'))
		{
			quote = str[*i];
			has_quote = 1;
			(*i)++;
			while (str[*i] && str[*i] != quote)
				(*i)++;
			if (str[*i] == quote)
				(*i)++;
		}
		(*i)++;
	}
	return ((void)(str[*i] && (*i)++), ft_substr(str, start, *i - start));
}

char	*get_quoted(const char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i])
		(*i)++;
	return (ft_substr(str, start, *i - start));
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
