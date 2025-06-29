/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	export(int argc, char **argv, t_envp **datas)
{
	t_envp	*actual;
	int		i;

	actual = *datas;
	i = 0;
	if (argc == 1)
	{
		env(*datas);
		return ;
	}
	if (datas == NULL)
		return ;
	while (actual->next != NULL)
		actual = actual->next;
	while (i < argc - 1)
	{
		if (ft_isalpha(argv[i + 1][0]) == 1 && ft_strchr(argv[i + 1], '='))
		{
			actual->next = add_cell(argv[i + 1]);
			actual = actual->next;
		}
		if (ft_isalpha(argv[i + 1][0]) == 0)
			printf("ERROR\n");
		i++;
	}
}
