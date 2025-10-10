/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_checker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execperm(t_main *main, char *pathname)
{
	if (access(pathname, X_OK) != 0)
	{
		printf("minishell: %s: Permission denied\n", main->cmd_info->cmd);
		main->last_exit_status = 126;
		fork_bad_file(main);
		return (1);
	}
	return (0);
}

int	ft_exist(t_main *main, char *pathname)
{
	if (access(pathname, F_OK) != 0)
	{
		printf("minishell: %s: No such file or directory", main->cmd_info->cmd);
		printf("\n");
		main->last_exit_status = 127;
		fork_bad_file(main);
		return (1);
	}
	return (0);
}

int	ft_isadirectory(t_main *main, char *pathname)
{
	struct stat	file_stat;

	if (stat(pathname, &file_stat) != 0)
		file_stat.st_mode = 0;
	if (S_ISDIR(file_stat.st_mode))
	{
		printf("minishell: %s: Is a directory\n", main->cmd_info->cmd);
		main->last_exit_status = 126;
		fork_bad_file(main);
		return (1);
	}
	return (0);
}

int	ft_commandisapoint(t_main *main)
{
	if (ft_strlen(main->cmd_info->cmd) == 1 && main->cmd_info->cmd[0] == '.')
	{
		printf("minishell:");
		printf(" %s: filename argument required\n", main->cmd_info->cmd);
		printf("%s: usage: . filename [arguments]\n", main->cmd_info->cmd);
		main->last_exit_status = 2;
		fork_bad_file(main);
		return (1);
	}
	return (0);
}

int	ft_commandonlypoints(t_main *main)
{
	if (isonlypt(main->cmd_info->cmd) && ft_strlen(main->cmd_info->cmd) > 1 \
		&& main->cmds_paths && main->cmds_paths->paths \
		&& main->cmds_paths->paths[0])
	{
		printf("minishell: %s: command not found\n", main->cmd_info->cmd);
		main->last_exit_status = 127;
		fork_bad_file(main);
		return (1);
	}
	return (0);
}
