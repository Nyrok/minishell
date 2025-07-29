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

static void	replace_word_env(t_envp *envp, char **word, char *key, size_t *i)
{
	char	*env_value;
	char	*before_word;
	char	*after_word;
	size_t	after_len;

	env_value = get_env_value(envp, key);
	if (!env_value)
		return ;
	before_word = ft_strjoin(ft_substr(*word, 0, *i - ft_strlen(key) - 1), \
		env_value);
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

static void	parse_word(t_envp *envp, char **word)
{
	char	*key;
	size_t	i;

	i = 0;
	while (word && (*word) && (*word)[i])
	{
		if ((*word)[i] == '$')
		{
			i++;
			key = get_key(*word, &i);
			replace_word_env(envp, word, key, &i);
		}
		i++;
	}
}

void	parse_env(t_envp *envp, t_token *tokens)
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
			parse_word(envp, &tokens->word);
		tokens = tokens->next;
		has_quote = 0;
	}
}
