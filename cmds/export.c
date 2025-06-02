/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmds.h"

t_export	*add_cell(char *str)
{
	t_export	*cell;

	cell = malloc(sizeof(t_export));
	cell->data = ft_strdup((const char *)str);
	//printf("%s\n", cell->data);
	cell->next = NULL;
	return (cell);
}

int	args_checker(int argc)
{
	if (argc != 2)
	{
		printf("1 argument !");
		return (0);
	}
	return (1);
}

int	export(int argc, char **argv, char **envp)
{
	t_export	*datas;
	t_export	*actual;
	int			i;

	i = 0;
	if (envp[0] == NULL || args_checker(argc) == 0)
		return (-1);
	datas = add_cell(envp[i]);
	actual = datas;
	while (envp[i] != NULL)
	{
		actual->next = add_cell(envp[i]);
		actual = actual->next;
		i++;
	}
	actual->next = add_cell(argv[1]);
	actual = datas;
	while (actual != NULL)
	{
		printf("%s\n", actual->data);
		actual = actual->next;
	}
	return (1);
}