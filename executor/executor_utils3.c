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
		if (fd_opener(main, (*main)->cmd_info->infile, -1, 0) == -1)
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

int	check_if_exist(t_main *main, int ispoint)
{
	auto int i = 0;
	auto int cmd_found = 0;
	auto char *tmp = NULL;
	if (!main->cmds_paths || i < 0)
		return (0);
	while (main->cmds_paths->paths && main->cmds_paths->paths[i]
		&& main->cmd_info->cmd[0] != '.')
	{
		tmp = paths_searcher(main->cmd_info->cmd,
				main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
		if (access(tmp, F_OK) == 0)
		{
			if (ispoint == 1 && access(tmp, X_OK) == 0)
				main->cmd_info->cmd_path = ft_strdup(tmp);
			cmd_found = 1;
			if (ispoint == 1 && access(tmp, X_OK) != 0)
				cmd_found = -1;
			break ;
		}
		free(tmp);
		tmp = NULL;
	}
	if (tmp)
		free(tmp);
	return (cmd_found);
}

void	print_not_found(t_main *main, int error_code, int cmd_found)
{
	struct stat	file_stat;

	if (error_code == NOTFOUND && cmd_found == 0)
	{
		if (stat(main->cmd_info->cmd_path, &file_stat) != 0)
			file_stat.st_mode = 0;
		if (S_ISDIR(file_stat.st_mode))
		{
			printf("minishell: %s: Is a directory\n", main->cmd_info->cmd_path);
			main->last_exit_status = 126;
		}
		else if (check_if_exist(main, 0) == 0
			|| ft_strlen(main->cmd_info->cmd) == 0)
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

int	totalcmds(char *cmd)
{
	int	i;
	int	total;

	i = 0;
	total = 1;
	while (cmd[i])
	{
		if (cmd[i] == '|')
			total++;
		i++;
	}
	return (total);
}
