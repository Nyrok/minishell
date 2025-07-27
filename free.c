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

void	free_cmd_info(t_cmd_info **commande_info)
{
	t_cmd_info	*cmd_tmp;
	t_redir		*redirs_tmp;
	int			i;

	if (commande_info == NULL || *commande_info == NULL)
		return ;
	cmd_tmp = *commande_info;
	while (cmd_tmp->redirs)
	{
		redirs_tmp = cmd_tmp->redirs;
		cmd_tmp->redirs = cmd_tmp->redirs->next;
		if (redirs_tmp->filename)
			free(redirs_tmp->filename);
		free(redirs_tmp);
	}
	if (cmd_tmp->argv)
	{
		i = 0;
		while (cmd_tmp->argv[i])
			free(cmd_tmp->argv[i++]);
		free(cmd_tmp->argv);
		cmd_tmp->argv = NULL;
	}
	free(cmd_tmp);
	cmd_tmp = NULL;
}

void	free_main(t_main *main_struct)
{
	t_envp			*tmp;
	t_history		*tmp2;

	while (main_struct->envp != NULL)
	{
		tmp = main_struct->envp;
		main_struct->envp = main_struct->envp->next;
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
