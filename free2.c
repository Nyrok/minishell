<<<<<<< HEAD
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

void	free_execve(t_main **main)
{
	free_envp(&(*main)->envp);
	if ((*main)->cmd_info && (*main)->cmd_info->cmd_path)
		free((*main)->cmd_info->cmd_path);
	if ((*main)->pids)
		free((*main)->pids);
	free((*main)->tube);
	free_all_cmd_info(main);
	free((*main)->str_envp);
	free_tokens(&(*main)->tokens);
	list_history_cleaner(*main);
	rl_clear_history();
	free(*main);
}
=======
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

void	free_execve(t_main **main)
{
	free_envp(&(*main)->envp);
	free((*main)->cmd_info->cmd_path);
	free((*main)->tube);
	free_all_cmd_info(main);
	free((*main)->str_envp);
	free_tokens(&(*main)->tokens);
	list_history_cleaner(*main);
	free(*main);
}

void	delete_tube(t_main *main)
{
	if (main->tube && main->tube->fd != -1)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
}
>>>>>>> 6f73384cbfeeebee337938968911f9b79152f7a4
