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
		printf("exec\n");
		user_input = readline(PURPLE "minishell>" RESET);
		main->tokens = tokenize_input(user_input);
		main->cmd_info = parse_tokens(main->tokens);
		free_tokens(main);
		//printf("CC\n");
		if (main->cmd_info)
		{
			if (main->history == NULL && user_input)
				main->history = list_history_init(user_input);
			else if (user_input)
				list_history_add(&main->history, user_input);
			if (user_input && *user_input)
				add_history(user_input);
			if (!user_input)
				break ;
			if (builtin_exec(main->cmd_info, &main->datas, main->history) == 0)
				executor(user_input, main->cmds_paths->paths, main);
		}
		free(user_input);
	}
	free_main(main);
}
