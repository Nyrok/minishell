/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:38:28 by hkonte            #+#    #+#             */
/*   Updated: 2025/07/22 15:38:44 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_key(char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i] && is_valid_env_char(str[*i]))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static void	replace_word_env(char *env_value, char **word, char *key, size_t *i)
{
	char	*after_word;
	size_t	after_len;

	auto char *tmp = ft_substr(*word, 0, *i - ft_strlen(key) - 1);
	auto char *before_word = ft_strjoin(tmp, env_value);
	free(tmp);
	after_len = ft_strlen(*word) - *i;
	if (after_len)
	{
		after_word = ft_substr(*word, *i, after_len);
		free(*word);
		*word = ft_strjoin(before_word, after_word);
		free(after_word);
		free(before_word);
	}
	else
	{
		free(*word);
		*word = before_word;
	}
	*i = ft_strlen(env_value) - 1;
	if ((int)ft_strlen(env_value) - 1 < 0)
		*i = 0;
}

void	parse_word_env(t_envp *envp, char **word, int last_exit_status)
{
	char	*key;
	size_t	i;
	char	*env_value;

	if (!ft_strchr(*word, '$'))
		return ;
	i = 0;
	while (word && (*word) && (*word)[i] && ft_strlen(*word) > 1)
	{
		if ((*word)[i] == '$')
		{
			if (++i && !is_valid_env_char((*word)[i]))
				continue ;
			key = get_key(*word, &i);
			if (ft_strcmp(key, "?") == 0)
				env_value = ft_itoa(last_exit_status);
			else
				env_value = ft_strdup(get_env_value(envp, key));
			replace_word_env(env_value, word, key, &i);
			free(env_value);
			free(key);
		}
		if ((*word)[i] && (*word)[i] != '$')
			i++;
	}
}

void	parse_env(t_envp *envp, char **str, int last_exit_status)
{
	auto int i = 0;
	auto int double_quote = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '"')
			double_quote = !double_quote;
		if (!double_quote && (*str)[i] == '\'' && ++i)
			while ((*str)[i] && (*str)[i] != '\'')
				i++;
		else if ((*str)[i] == '$')
		{
			auto int start = i++;
			while ((*str)[i] && is_valid_env_char((*str)[i]))
				i++;
			auto char *to_expand = ft_substr(*str, start, i - start);
			parse_word_env(envp, &to_expand, last_exit_status);
			auto char *prefix = ft_substr(*str, 0, start);
			auto char *suffix = ft_strdup(*str + i);
			i = start + ft_strlen(to_expand);
			*str = three_strjoin(*str, prefix, to_expand, suffix);
			continue ;
		}
		if ((*str)[i])
			i++;
	}
}
