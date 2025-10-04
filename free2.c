/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	free_redir(t_redir **redir)
{
	t_redir		*redir_to_free;

	redir_to_free = *redir;
	*redir = (*redir)->next;
	if (redir_to_free->filename)
		free(redir_to_free->filename);
	if (redir_to_free->fd != -1)
		close(redir_to_free->fd);
	free(redir_to_free);
}

int	free_execve(t_main **main)
{
	delete_tube(*main);
	free_envp(&(*main)->envp);
	if ((*main)->cmd_info && (*main)->cmd_info->cmd_path)
	{
		free((*main)->cmd_info->cmd_path);
		(*main)->cmd_info->cmd_path = NULL;
	}
	if ((*main)->pids)
		free((*main)->pids);
	free((*main)->tube);
	free_all_cmd_info(main);
	free((*main)->str_envp);
	free_tokens(&(*main)->tokens);
	if ((*main)->cmds_paths)
	{
		auto int i = 0;
		while ((*main)->cmds_paths->paths && (*main)->cmds_paths->paths[i])
			free((*main)->cmds_paths->paths[i++]);
		free((*main)->cmds_paths->paths);
		free((*main)->cmds_paths);
	}
	list_history_cleaner(*main);
	rl_clear_history();
	auto int exit_code = (*main)->last_exit_status;
	free(*main);
	return (exit_code);
}

void	delete_tube(t_main *main)
{
	if (main->tube && main->tube->fd != -1)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
}
