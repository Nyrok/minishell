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

void	add_or_replace(t_main *main, t_envp **actual, char *key, char *value)
{
	t_envp	**curr;
	char	*full;

	curr = get_env_addr(&main->envp, key);
	full = ft_strjoin(key, "=");
	if (!full)
		return ;
	if (!curr)
	{
		(*actual)->next = add_cell(key, value);
		*actual = (*actual)->next;
	}
	else
	{
		free((*curr)->key);
		free((*curr)->value);
		free((*curr)->full);
		(*curr)->key = key;
		(*curr)->value = value;
		(*curr)->full = ft_strjoin(full, value);
	}
	free(full);
}

int	export(t_main *main, int argc, char **argv, int nbcmds)
{
	auto t_envp * actual = main->envp;
	auto int i = 0;
	if (argc == 1)
		env(main, main->envp, nbcmds);
	reset_tube(main);
	if (main->envp == NULL)
		return (-1);
	while (actual->next != NULL)
		actual = actual->next;
	while (++i < argc)
	{
		auto char **pair = ft_split(argv[i], '=');
		if (!pair || !is_valid_env_name(pair[0]))
			printf("-minishell: Invalid env name\n");
		else if (!ft_strchr(argv[i], '='))
			printf("-minishell: Missing env value\n");
		else
			add_or_replace(main, &actual, pair[0], pair[1]);
	}
	return (1);
}
