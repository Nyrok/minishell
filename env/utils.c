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

int	is_valid_env_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	is_valid_env_name(char *str)
{
	if (!str)
		return (0);
	if (ft_isdigit(*str))
		return (0);
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
