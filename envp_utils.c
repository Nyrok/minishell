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

#include "cmds/cmds.h"

t_export	*add_cell(char *str)
{
	t_export	*cell;

	cell = malloc(sizeof(t_export));
	cell->data = ft_strdup((const char *)str);
	cell->next = NULL;
	return (cell);
}

t_export	*list_maker(char **envp)
{
	t_export	*datas;
	t_export	*actual;
	char		*key;
	int			i;

	i = 0;
	datas = add_cell(envp[i]);
	actual = datas;
	key = data_spliter(envp[i]);
	while (envp[i] != NULL)
	{
		actual->next = add_cell(envp[i]);
		actual = actual->next;
		i++;
	}
	free(key);
	actual = NULL;
	return (datas);
}
