/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	line_reader(t_main *main)
{
	char		*user_input;

	while (1)
	{
		user_input = readline(PURPLE "minishell>" RESET);
		if (main->history == NULL)
			main->history = list_history_init(user_input);
		else
			list_history_add(&main->history, user_input);
		if (user_input && *user_input)
			add_history(user_input);
		if (!user_input)
			anti_leaks(&main);
		if (check_cmds(user_input, &main->datas, main->history) == 0)
			cmd_searcher(user_input, main->cmds_paths->paths, main->datas);
		free(user_input);
	}
}
