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
	if (argc == 1 && nbcmds == 1)
		print_env(STDOUT_FILENO, main->envp, 1);
	delete_tube(main);
	if (main->envp == NULL)
		return (-1);
	while (actual->next != NULL)
		actual = actual->next;
	while (argv && ++i < argc)
	{
		if (!ft_strchr(argv[i], '='))
			continue ;
		auto char **pair = ft_split_env(argv[i]);
		if (!pair || !is_valid_env_name(pair[0]))
			printf("-minishell: Invalid env name\n");
		else
			add_or_replace(main, &actual, \
				ft_strdup(pair[0]), ft_strdup(pair[1]));
		free_split(pair, 2);
	}
	main->last_exit_status = 0;
	return (1);
}
