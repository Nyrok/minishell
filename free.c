/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_main *main_struct)
{
	t_token	*last_token;
	t_token	*current_token;

	last_token = main_struct->tokens;
	current_token = last_token->next;
	while (current_token)
	{
		free(last_token);
		last_token = current_token;
		current_token = current_token->next;
	}
	free(last_token);
}

void	free_main(t_main *main_struct)
{
	t_envp			*tmp;
	t_history		*tmp2;

	while (main_struct->datas != NULL)
	{
		tmp = main_struct->datas;
		main_struct->datas = main_struct->datas->next;
		free(tmp);
	}
	while (main_struct->history != NULL)
	{
		tmp2 = main_struct->history;
		main_struct->history = main_struct->history->next;
		free(tmp2->cmd);
		free(tmp2);
	}
	rl_clear_history();
	free(main_struct);
}
