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

void		free_redir(t_redir **redir);

// void	free_all_cmd_info(t_main **main)
// {
// 	t_cmd_info	*cmd_tmp;
// 	t_cmd_info	*to_free;
// 	t_redir		*redir_tmp;
// 	int			i;

// 	cmd_tmp = (*main)->cmd_info;
// 	while (cmd_tmp)
// 	{
// 		redir_tmp = cmd_tmp->redirs;
// 		while (redir_tmp)
// 			free_redir(&redir_tmp);
// 		i = 0;
// 		if (cmd_tmp->argv)
// 		{
// 			while (cmd_tmp->argv[i])
// 				free(cmd_tmp->argv[i++]);
// 			free(cmd_tmp->argv);
// 		}
// 		if (cmd_tmp->cmd)
// 			free(cmd_tmp->cmd);
// 		to_free = cmd_tmp;
// 		cmd_tmp = cmd_tmp->next;
// 		free(to_free);
// 	}
// 	(*main)->cmd_info = NULL;
// }

void	free_all_cmd_info(t_main **main)
{
	t_cmd_info	*cmd_tmp;
	t_cmd_info	*to_free;

	cmd_tmp = (*main)->cmd_info;
	while (cmd_tmp)
	{
		to_free = cmd_tmp;
		cmd_tmp = cmd_tmp->next;
		free_cmd_info(&to_free);
	}
	(*main)->cmd_info = NULL;
}

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
	*tokens = NULL;
}

void	free_cmd_info(t_cmd_info **cmd_info)
{
	t_cmd_info	*cmd_tmp;
	t_redir		*redirs_tmp;
	int			i;

	if (cmd_info == NULL || *cmd_info == NULL)
		return ;
	cmd_tmp = *cmd_info;
	while (cmd_tmp->redirs)
	{
		redirs_tmp = cmd_tmp->redirs;
		cmd_tmp->redirs = cmd_tmp->redirs->next;
		if (redirs_tmp->filename)
			free(redirs_tmp->filename);
		if (redirs_tmp->fd != -1)
			close(redirs_tmp->fd);
		free(redirs_tmp);
	}
	i = 0;
	while (cmd_tmp->argv && cmd_tmp->argv[i])
		free(cmd_tmp->argv[i++]);
	free(cmd_tmp->cmd);
	free(cmd_tmp->argv);
	free(cmd_tmp);
	*cmd_info = NULL;
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
