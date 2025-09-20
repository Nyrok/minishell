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

	auto char *env_value = get_env_value(envp, key);
	if (!env_value)
		return ;
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
}

static void	parse_word(t_envp *envp, char **word, int last_exit_status)
{
	char	*key;
	size_t	i;
	char	*env_value;

	i = 0;
	while (word && (*word) && (*word)[i])
	{
		if ((*word)[i] == '$')
		{
			i++;
			key = get_key(*word, &i);
			if (ft_strcmp(key, "?") == 0)
				env_value = ft_itoa(last_exit_status);
			else
				env_value = get_env_value(envp, key);
			replace_word_env(env_value, word, key, &i);
			free(key);
		}
		if ((*word)[i] && (*word)[i] != '$')
			i++;
	}
}

void	parse_env(t_envp *envp, t_token *tokens, int last_exit_status)
{
	int		has_quote;

	has_quote = 0;
	while (tokens)
	{
		if (tokens->word)
		{
			has_quote = tokens->word[0] == '"';
			tokens->word = rm_quotes(tokens->word, 0, \
				ft_strlen(tokens->word));
		}
		if (tokens->type == WORD && tokens->word \
			&& (tokens->word[0] == '$' || has_quote))
			parse_word(envp, &tokens->word, last_exit_status);
		tokens = tokens->next;
		has_quote = 0;
	}
}
