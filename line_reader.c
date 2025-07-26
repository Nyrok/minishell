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
	t_envp		*tmp;
	t_history	*hist_tmp;

	while (main->envp != NULL)
	{
		tmp = main->envp;
		free(tmp->key);
		free(tmp->value);
		free(tmp->full);
		main->envp = main->envp->next;
		free(tmp);
	}
	while (main->history != NULL)
	{
		hist_tmp = main->history;
		main->history = main->history->next;
		if (hist_tmp->cmd)
			free(hist_tmp->cmd);
		free(hist_tmp);
	}
	if (main->cmd_info)
		free_cmd_info(&main->cmd_info);
	main->cmd_info = NULL;
	free(main->history);
	free(main->envp);
	free(main);
	exit(1);
}

void	line_reader(t_main *main)
{
	char		*user_input;

	while (1)
	{
		user_input = readline(PURPLE "minishell>" RESET);
		if (user_input == NULL)
			ft_ctrld(main); // ajouter les free
		main->tokens = tokenize_input(user_input);
		main->cmd_info = parse_tokens(main, main->tokens);
		free_tokens(main);
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
	}
	free_main(main);
}
