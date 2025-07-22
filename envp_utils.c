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

t_envp	*add_cell(char *str)
{
	t_envp	*cell;

	cell = malloc(sizeof(t_envp));
	if (cell == NULL)
		return (NULL);
	cell->data = str;
	cell->next = NULL;
	return (cell);
}

t_envp	*list_maker(char **envp)
{
	t_envp		*curr_envp;
	t_envp		*actual;
	char		*key;
	int			i;

	i = 0;
	if (envp[i] == NULL)
		return (NULL);
	curr_envp = add_cell(envp[i]);
	actual = curr_envp;
	key = data_spliter(envp[i]);
	while (envp[i] != NULL)
	{
		actual->next = add_cell(envp[i]);
		actual = actual->next;
		i++;
	}
	actual->next = NULL;
	free(key);
	return (curr_envp);
}

char	*get_env_value(t_envp *envp, char *key)
{
	char	**pair;

	while (envp)
	{
		pair = ft_split(envp->data, '=');
		if (pair && ft_strcmp(pair[0], key) == 0)
			return (pair[1]);
		envp = envp->next;
	}
	return (NULL);
}

char	**envp_to_str(t_envp *envp)
{
	t_envp		*curr_envp;
	char		**res;
	int			i;

	i = 0;
	curr_envp = envp;
	while (curr_envp)
	{
		i++;
		curr_envp = curr_envp->next;
	}
	curr_envp = envp;
	res = malloc((i + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (curr_envp)
	{
		res[i] = ft_strdup(curr_envp->data);
		curr_envp = curr_envp->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}
