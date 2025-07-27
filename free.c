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

void	free_envp(t_envp **envp)
{
	t_envp	*last_envp;
	t_envp	*current_envp;

	last_envp = *envp;
	if (last_envp)
		current_envp = last_envp->next;
	while (last_envp)
	{
		free(last_envp->key);
		free(last_envp->value);
		free(last_envp->full);
		free(last_envp);
		last_envp = current_envp;
		if (current_envp)
			current_envp = current_envp->next;
	}
}

void	free_tokens(t_token **tokens)
{
	t_token	*last_token;
	t_token	*current_token;

	last_token = *tokens;
	if (last_token)
		current_token = last_token->next;
	while (last_token)
	{
		free(last_token->word);
		free(last_token);
		last_token = current_token;
		if (current_token)
			current_token = current_token->next;
	}
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
	i = 0;
	while (cmd_tmp->argv && cmd_tmp->argv[i])
		free(cmd_tmp->argv[i++]);
	free(cmd_tmp->cmd);
	free(cmd_tmp->cmd_path);
	free(cmd_tmp->argv);
	free(cmd_tmp);
}

void	free_main(t_main **main_struct)
{
	t_history		*tmp;

	free_envp(&(*main_struct)->envp);
	while ((*main_struct)->history != NULL)
	{
		tmp = (*main_struct)->history;
		(*main_struct)->history = (*main_struct)->history->next;
		free(tmp->cmd);
		free(tmp);
	}
	rl_clear_history();
	free(*main_struct);
}
