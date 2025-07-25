/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envp	*create_envp(char *key, char *value)
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
	free(temp);
	cell->next = NULL;
	return (cell);
}

void	add_or_replace(t_main *main, t_envp **actual, char *key, char *value)
{
	t_envp	**curr;
	char	*full;

	curr = get_env_addr(&main->envp, key);
	full = ft_strjoin(key, "=");
	if (!full)
		return ;
	if (!curr)
	{
		(*actual)->next = create_envp(key, value);
		*actual = (*actual)->next;
	}
	else
	{
		free((*curr)->key);
		free((*curr)->value);
		free((*curr)->full);
		(*curr)->key = key;
		(*curr)->value = value;
		(*curr)->full = ft_strjoin(full, value);
	}
	free(full);
}

t_envp	*init_env(char **envp)
{
	t_envp		*head;
	t_envp		*actual;
	char		**pair;

	auto int i = -1;
	if (!envp || !envp[0])
		return (NULL);
	actual = NULL;
	head = NULL;
	while (envp[++i] != NULL)
	{
		pair = ft_split(envp[i], '=');
		if (!head)
		{
			actual = create_envp(pair[0], pair[1]);
			head = actual;
		}
		else if (actual)
		{
			actual->next = create_envp(pair[0], pair[1]);
			actual = actual->next;
		}
		free(pair);
	}
	return (head);
}

t_envp	**get_env_addr(t_envp **envp, char *key)
{
	while (*envp)
	{
		if (ft_strcmp((*envp)->key, key) == 0)
		{
			return (envp);
		}
		envp = &(*envp)->next;
	}
	return (NULL);
}
