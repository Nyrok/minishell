/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_name(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isalpha(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

t_envp	*add_cell(char *key, char *value)
{
	t_envp	*cell;
	char	*temp;

	cell = malloc(sizeof(t_envp));
	if (cell == NULL)
		return (NULL);
	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	cell->key = key;
	cell->value = value;
	cell->full = ft_strjoin(temp, value);
	cell->next = NULL;
	return (cell);
}

t_envp	*list_maker(char **envp)
{
	t_envp		*curr_envp;
	t_envp		*actual;
	char		**pair;
	int			i;

	i = 0;
	if (envp[i] == NULL)
		return (NULL);
	pair = ft_split(envp[i], '=');
	curr_envp = add_cell(pair[0], pair[1]);
	actual = curr_envp;
	while (envp != NULL)
	{
		actual->next = add_cell(pair[0], pair[1]);
		actual = actual->next;
		i++;
	}
	actual->next = NULL;
	free(pair);
	return (curr_envp);
}

char	*get_env_value(t_envp *envp, char *key)
{
	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
		{
			return (envp->value);
		}
		envp = envp->next;
	}
	return (NULL);
}

char	**envp_to_str(t_envp *envp)
{
	auto int i = 0;
	auto t_envp		*curr_envp = envp;
	while (curr_envp)
	{
		i++;
		curr_envp = curr_envp->next;
	}
	curr_envp = envp;
	char		**res = malloc((i + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (curr_envp)
	{
		res[i] = ft_strdup(curr_envp->full);
		curr_envp = curr_envp->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}
