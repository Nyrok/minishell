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
	t_envp		*datas;
	t_envp		*actual;
	char		*key;
	int			i;

	i = 0;
	if (envp[i] == NULL)
		return (NULL);
	datas = add_cell(envp[i]);
	actual = datas;
	key = data_spliter(envp[i]);
	while (envp[i] != NULL)
	{
		actual->next = add_cell(envp[i]);
		actual = actual->next;
		i++;
	}
	actual->next = NULL;
	free(key);
	return (datas);
}

char	**envp_to_str(t_envp *envp)
{
	t_envp		*datas;
	char		**res;
	int			i;

	i = 0;
	datas = envp;
	while (datas)
	{
		i++;
		datas = datas->next;
	}
	datas = envp;
	res = malloc((i + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (datas)
	{
		res[i] = ft_strdup(datas->data);
		datas = datas->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}
