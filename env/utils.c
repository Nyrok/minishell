/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split_env(char const *s)
{
	auto int i = 0;
	auto int j = 0;
	auto char *c = ft_strchr(s, '=');
	auto char **res = ft_calloc(sizeof(char *), 3);
	if (!res)
		return (NULL);
	auto int has_found = 0;
	while (s && s[i] && j < !!c + 1)
	{
		auto int k = 0;
		while ((has_found || s + i + k != c) && s[i + k + 1])
			k++;
		has_found = 1;
		if (has_found && s + i == c)
			i++;
		res[j] = add_word(s, i, k);
		if (!res[j])
			return (free_split(res, j), NULL);
		i += k;
		j++;
	}
	return (res);
}

int	is_valid_env_char(char c)
{
	return (ft_isalpha(c) || c == '_' || c == '?');
}

int	is_valid_env_name(char *str)
{
	if (!str)
		return (0);
	if (!ft_strlen(str))
		return (0);
	if (ft_isdigit(*str))
		return (0);
	if (ft_strlen(str) == 1 && *str == '?')
		return (1);
	while (*str)
	{
		if (!is_valid_env_char(*str))
			return (0);
		str++;
	}
	return (1);
}

char	*get_env_value(t_envp *envp, char *key)
{
	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
			return (envp->value);
		envp = envp->next;
	}
	return (NULL);
}

char	**envp_to_str(t_envp *envp)
{
	auto int i = 0;
	auto t_envp * curr_envp = envp;
	while (curr_envp)
	{
		i++;
		curr_envp = curr_envp->next;
	}
	curr_envp = envp;
	auto char **res = malloc((i + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (curr_envp)
	{
		res[i] = curr_envp->full;
		curr_envp = curr_envp->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}
