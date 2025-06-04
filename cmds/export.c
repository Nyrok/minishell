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


#include "../minishell.h"

void	export(int argc, char **argv, t_export **datas)
{
	t_export	*actual;
	int			i;

	actual = *datas;
	i = 0;
	if (datas == NULL || argc == 1)
		return ;
	while (actual->next != NULL)
		actual = actual->next;
	while (i < argc - 1)
	{
		actual->next = add_cell(argv[i + 1]);
		actual = actual->next;
		i++;
	}
}
