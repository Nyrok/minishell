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
	while (str[*i] && !ft_isspace(str[*i]) && str[*i] != '/')
		(*i)++;
	return (ft_substr(str, start, *i));
}

static void	replace_word_env(t_envp *envp, char **word, char *key, size_t i)
{
	char	*env_value;
	char	*new_word;

	env_value = get_env_value(envp, key);
	printf("KEY %s VALUE %s\n", key, env_value);
	if (!env_value)
		return ;
	new_word = ft_strjoin(env_value, ft_substr(*word, i, ft_strlen(*word)));
	free(*word);
	*word = new_word;
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
			key = get_key(*word, &i);
			replace_word_env(envp, word, key, i);
		}
		i++;
	}
}

void	parse_env(t_envp *envp, t_token **tokens)
{
	t_token	*start;

	start = *tokens;
	while (*tokens)
	{
		if ((*tokens)->type == WORD && (*tokens)->word)
			parse_word(envp, &(*tokens)->word);
		*tokens = (*tokens)->next;
	}
	*tokens = start;
}
