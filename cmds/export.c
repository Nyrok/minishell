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

int	export(t_main *main, int argc, char **argv, int nbcmds)
{
	t_envp	*actual;

	actual = main->datas;
	auto int i = 0;
	if (argc == 1)
		env(main, main->datas, nbcmds);
	if (main->tube != NULL && main->tube->fd >= 0)
		close(main->tube->fd);
	main->tube = NULL;
	if (main->datas == NULL)
		return (-1);
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
	return (1);
}
