/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fd_opener(t_main **main, t_redir *actual_redir, int error_check)
{
	if (actual_redir->io == STDOUT_FILENO \
		&& access(actual_redir->filename, F_OK) != 0)
		actual_redir->fd = open(actual_redir->filename, O_CREAT, 0777); /* Leak de FD /!\ */
	if (actual_redir->io == STDIN_FILENO \
		&& access(actual_redir->filename, F_OK) != 0 \
			&& actual_redir->type != HEREDOC)
		return (no_such_file(actual_redir));
	else if (access(actual_redir->filename, R_OK) != 0 && \
			actual_redir->type != HEREDOC)
		return (permission_denied(main, actual_redir));
	if (actual_redir->type == APPEND)
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (actual_redir->type == REDIN)
		actual_redir->fd = open(actual_redir->filename, O_RDONLY, 0444);
	else if (actual_redir->type == HEREDOC && error_check == 0)
		actual_redir->fd = ft_heredoc(actual_redir->filename);
	else
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (actual_redir->fd == -1)
		return (-1);
	if (actual_redir->good == 0)
		close(actual_redir->fd);
	return (1);
}

void	check_tube(t_main **main)
{
	if ((*main)->tube == NULL)
	{
		(*main)->pids = ft_calloc((count_cmd_info((*main)->cmd_info) + 1), \
			sizeof(pid_t));
		setup_tube(*main);
	}
}

int	create_out(t_main *main)
{
	t_redir	*redir_tmp;

	while (main->cmd_info->redirs)
	{
		redir_tmp = main->cmd_info->redirs;
		main->cmd_info->redirs = main->cmd_info->redirs->next;
		if (redir_tmp->type == REDOUT)
		{
			redir_tmp->fd = open(redir_tmp->filename,
					O_CREAT | O_WRONLY | O_APPEND, 0777);
			if (redir_tmp->fd == -1)
				return (-1);
			close(redir_tmp->fd);
		}
		free(redir_tmp->filename);
		free(redir_tmp);
	}
	return (1);
}

int	handle_heredoc(t_main *main)
{
	t_redir	*redir_tmp;
	int		fd;

	redir_tmp = main->cmd_info->redirs;
	while (redir_tmp)
	{
		if (redir_tmp->type == HEREDOC)
		{
			fd = ft_heredoc(redir_tmp->filename);
			if (fd != -1)
			{
				close(fd);
				return (-1);
			}
		}
		redir_tmp = redir_tmp->next;
	}
	return (-1);
}

int	multiple_cmd_handler(t_main *main, char **envp, int nbcmds)
{
	t_cmd_info	*temp_cmd_info;

	if (nbcmds == 0)
		free_cmd_info(&main->cmd_info);
	while (nbcmds > 0)
	{
		if (main->cmd_info->cmd == NULL)
			return (handle_heredoc(main), create_out(main), free_all_cmd_info(&main),end_pids(&main), no_leaks(main), -1);
		setup_cmd_redirs(main->cmd_info);
		check_tube(&main);
		if (hasinfile(&main, 1) == -1)
			return (free_all_cmd_info(&main), fdcls(&main, 1), no_leaks(main), 0); // end_pids(&main, pids) retiré
		fdcls(&main, 0);
		multiplecmdexector(main, envp, nbcmds--);
		temp_cmd_info = main->cmd_info;
		main->cmd_info = main->cmd_info->next;
		free_cmd_info(&temp_cmd_info);
	}
	main->cmd_info = NULL;
	delete_tube(main);
	return (1);
}
