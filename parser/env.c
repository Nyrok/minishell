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

static char	*replace_word_env(t_envp *envp, char *str, char *key, size_t i)
{
	char	*env_value;

	env_value = get_env_value(envp, key);
	if (!env_value)
		return (str);
	return (ft_strjoin(env_value, ft_substr(str, i, ft_strlen(str))));
}

void	parse_env(t_envp *envp, t_token **tokens)
{
	char	*word;
	char	*key;
	size_t	i;
	t_token	*start;

	start = *tokens;
	while (*tokens)
	{
		if ((*tokens)->type == WORD && (*tokens)->word \
		&& (*tokens)->word[0] == '$')
		{
			i = 1;
			key = get_key((*tokens)->word, &i);
			word = replace_word_env(envp, (*tokens)->word, key, i);
			free((*tokens)->word);
			(*tokens)->word = word;
		}
		*tokens = (*tokens)->next;
	}
	*tokens = start;
}
