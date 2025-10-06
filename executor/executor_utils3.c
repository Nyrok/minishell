/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tube_handler(t_main **main)
{
	if ((*main)->cmd_info->infile != NULL)
	{
		if (fd_opener(main, (*main)->cmd_info->infile, 0, 0) == -1)
			return (-1);
		if ((*main)->tube->fd != -1
			&& (*main)->cmd_info->infile->fd != (*main)->tube->fd)
		{
			close((*main)->tube->fd);
			(*main)->tube->fd = -1;
		}
	}
	return (1);
}

void	print_not_found(t_main *main, int error_code, int cmd_found)
{
	struct stat	file_stat;

	if (error_code == NOTFOUND && cmd_found == 0)
	{
		if (stat(main->cmd_info->cmd_path, &file_stat) != 0)
			file_stat.st_mode = 0;
		if (S_ISDIR(file_stat.st_mode) \
			&& ft_strlen(main->cmd_info->cmd) != 0)
		{
			printf("minishell: %s: Is a directory\n", main->cmd_info->cmd);
			main->last_exit_status = 126;
		}
		else if (ft_strlen(main->cmd_info->cmd) == 0 \
			|| check_if_exist(main) == 0)
		{
			printf("minishell: %s: command not found\n", main->cmd_info->cmd);
			main->last_exit_status = 127;
		}
		else if (access(main->cmd_info->cmd_path, X_OK) != 0)
		{
			printf("minishell: %s: Permission denied\n", main->cmd_info->cmd);
			main->last_exit_status = 126;
		}
	}
}

int	print_error(t_main *main, int error_code, int cmd_found)
{
	if (error_code == NOTFOUND && cmd_found == 0)
		print_not_found(main, error_code, cmd_found);
	else if (error_code == DEF_PIPE && cmd_found == 0)
	{
		if (main->tube && main->tube->fd != -1)
		{
			close(main->tube->fd);
			main->tube->fd = -1;
		}
		perror("pipe");
		return (-1);
	}
	return (1);
}

int	no_such_file(t_redir *actual_redir, int print)
{
	if (print == 1)
		printf("-minishell: %s: No such file or directory\n",
			actual_redir->filename);
	return (0);
}

int	permission_denied(t_main **main, t_redir *actual_redir, int print)
{
	if (actual_redir->type == REDOUT
		&& access(actual_redir->filename, F_OK) != 0)
	{
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
		return (0);
	}
	if (print == 1)
		printf("-minishell: %s: Permission denied\n", actual_redir->filename);
	(*main)->last_exit_status = 126;
	if (ft_strcmp((*main)->cmd_info->cmd, "echo") == 0)
		(*main)->last_exit_status = 1;
	return (0);
}
