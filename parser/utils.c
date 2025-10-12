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

char	*rm_char(char *str, const char c)
{
	size_t	i;
	size_t	j;
	int		count;
	char	*result;

	if (!str)
		return (str);
	count = 0;
	i = -1;
	while (++i < ft_strlen(str))
		if (str[i] == c)
			count++;
	result = ft_calloc(ft_strlen(str) - count + 1, sizeof(char));
	if (!result)
		return (str);
	i = -1;
	j = 0;
	while (++i < ft_strlen(str))
		if (str[i] != c)
			result[j++] = str[i];
	free(str);
	return (result);
}

char	*rm_dollars(char *str, size_t start, size_t n)
{
	size_t	i;
	size_t	j;
	int		count;
	char	*result;

	if (!str)
		return (str);
	count = 0;
	i = -1;
	while (++i < n)
		if (str[start + i] == '$' && str[start + i + 1] \
			&& (is_valid_env_char(str[start + i + 1]) \
		|| ft_strchr("'\"", str[start + i + 1])))
			count++;
	result = ft_calloc(n - count + 1, sizeof(char));
	i = -1;
	j = 0;
	while (result && ++i < n)
		if (str[start + i] != '$' || !str[start + i + 1] \
			|| (!is_valid_env_char(str[start + i + 1]) \
			&& !ft_strchr("'\"", str[start + i + 1])))
			result[j++] = str[start + i];
	if (result)
		free(str);
	return (result);
}

char	*get_word(const char *str, size_t *i)
{
	size_t	start;
	char	quote;

	start = *i;
	quote = 0;
	while (str[*i])
	{
		if (!quote && (ft_isspace(str[*i]) || ft_strchr("|<>", str[*i])) \
		&& (*i)--)
			break ;
		if ((str[*i] == '\'' || str[*i] == '"'))
		{
			quote = str[*i];
			(*i)++;
			while (str[*i] && (str[*i] != quote \
				|| (str[*i + 1] && !ft_isspace(str[*i + 1]))))
				(*i)++;
			if (str[*i] == quote \
				&& (!str[*i + 1] || ft_isspace(str[*i + 1])))
				break ;
		}
		if (str[*i])
			(*i)++;
	}
	return ((void)(str[*i] && (*i)++), ft_substr(str, start, *i - start));
}

char	*get_quoted(const char *str, size_t *i)
{
	size_t	start;
	char	quote;

	start = *i;
	quote = str[start];
	(*i)++;
	while (str[*i] && (str[*i] != quote \
	|| (str[*i + 1] && !ft_isspace(str[*i + 1]))))
		(*i)++;
	if (str[*i] == quote
		&& (!str[*i + 1] || ft_isspace(str[*i + 1])))
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
