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

void	ft_ctrld(t_main *main)
{
	if (main->tube)
	{
		if (main->tube->fd != -1)
			close(main->tube->fd);
		free(main->tube);
		main->tube = NULL;
	}
	if (main->cmd_info)
		free_cmd_info(&main->cmd_info);
	if (main->str_envp)
		free(main->str_envp);
	free_main(&main);
	exit(EXIT_SUCCESS);
}

void	line_reader(t_main *main)
{
	char	*user_input;

	while (1)
	{
		user_input = readline(PURPLE "minishell>" RESET);
		if (user_input == NULL)
			ft_ctrld(main);
		main->tokens = tokenize_input(user_input);
		main->cmd_info = parse_tokens(main, main->tokens);
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
			executor(user_input, main);
		}
		free(user_input);
		free_cmd_info(&main->cmd_info);
		free_tokens(&main->tokens);
		printf("Exit status : %d\n", main->last_exit_status);
	}
}
