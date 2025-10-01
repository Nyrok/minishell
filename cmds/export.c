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
	auto t_envp * actual = main->envp;
	auto int i = 0;
	if (argc == 1)
		env(main, main->envp, 1, nbcmds);
	delete_tube(main);
	if (main->envp == NULL)
		return (-1);
	while (actual->next != NULL)
		actual = actual->next;
	while (argv && ++i < argc)
	{
		auto char **pair = ft_split_env(argv[i]);
		if (!pair || !is_valid_env_name(pair[0]))
			printf("-minishell: Invalid env name\n");
		else if (!ft_strchr(argv[i], '='))
			printf("-minishell: Missing env value\n");
		else
			add_or_replace(main, &actual, \
				ft_strdup(pair[0]), ft_strdup(pair[1]));
		free_split(pair, 2);
	}
	return (1);
}
