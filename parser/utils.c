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

char	*rm_nchar(char *str, const char c, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	count;
	char	*result;

	if (!str)
		return (str);
	count = 0;
	i = -1;
	while (++i < ft_strlen(str) && count < n)
		if (str[i] == c)
			count++;
	result = ft_calloc(ft_strlen(str) - count + 1, sizeof(char));
	if (!result)
		return (str);
	i = -1;
	j = 0;
	while (++i < ft_strlen(str) && count < n)
		if (str[i] != c)
			result[j++] = str[i];
	free(str);
	return (result);
}

char	*three_strjoin(char *old, char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	free(s1);
	free(s2);
	result = ft_strjoin(tmp, s3);
	if (!result)
		return (NULL);
	free(tmp);
	free(s3);
	free(old);
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
	auto size_t start = *i;
	auto char quote = 0;
	while (str[*i])
	{
		if (!quote && (ft_isspace(str[*i]) || ft_strchr("|<>", str[*i])) \
		&& (*i)--)
			break ;
		if (!quote && ft_strchr("'\"", str[*i]))
		{
			quote = str[*i];
			(*i)++;
			while (str[*i] && str[*i] != quote)
				(*i)++;
			if (str[*i] == quote)
				quote = 0;
		}
		if (str[*i])
			(*i)++;
	}
	return ((void)(str[*i] && (*i)++), ft_substr(str, start, *i - start));
}

void	parse_quotes(char **str)
{
	auto int i = 0;
	auto int quote = -1;
	while ((*str)[i])
	{
		while (quote == -1 && ft_isspace((*str)[i]))
			i++;
		if (quote > -1 && (*str)[quote] == (*str)[i])
		{
			*str = three_strjoin(*str, ft_substr(*str, 0, quote), \
				ft_substr(*str, quote + 1, i - quote - 1), \
				ft_strdup(*str + i + 1));
			i--;
			quote = -1;
		}
		if (quote == -1)
		{
			if (ft_strchr("'\"", (*str)[i]))
				quote = i;
			else if (ft_strchr("|<>", (*str)[i]))
				break ;
		}
		if ((*str)[i])
			i++;
	}
}
