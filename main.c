/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cmds/cmds.h"

int	main(int argc, char **argv, char **envp)
{
	t_export	*datas;
	t_export	*actual;

	datas = list_maker(argc, argv, envp);
	actual = datas;
	while (actual != NULL)
	{
		printf("%s\n", actual->data);
		actual = actual->next;
	}
	export(argc, argv, &datas);
	printf("\n\n\n\n\n");
	actual = datas;
	while (actual != NULL)
	{
		printf("%s\n", actual->data);
		actual = actual->next;
	}
	argv[1] = "LOL";
	printf("\n\n\n===UNSET===\n\n\n");
	unset(argc, argv, &datas);
	actual = datas;
	while (actual != NULL)
	{
		printf("%s\n", actual->data);
		actual = actual->next;
	}
}
