/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_searcher.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	llaunch_executions(t_main *main, char **envp, int tube, int i)
{
	if (i == -2)
	{
		free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
	}
	last_executor(main, envp, tube, i);
	free(main->cmd_info->cmd_path);
	main->cmd_info->cmd_path = NULL;
	return (1);
}

static void	handle_null_case(t_main *main, char **envp, int tube, int i)
{
	ft_strdup_cmd_path(main, i);
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] != '/')
		last_executor(main, envp, tube, i);
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] == '/')
		llaunch_executions(main, envp, tube, -2);
}

void	lcmd_searcher(t_main *main, char **envp, int tube)
{
	auto int i = 0;
	auto int cmd_found = 0;
	cmds_paths_maker(main);
	if (ft_strchr(main->cmd_info->cmd, '/') || !main->cmds_paths \
		|| !main->cmds_paths->paths || !main->cmds_paths->paths[0] \
		|| ft_strchr(main->cmd_info->cmd, '.'))
		relative_path_executor(main, envp, 1);
	else
	{
		while (main->cmds_paths->paths && main->cmds_paths->paths[i] \
			&& cmd_found != -1)
		{
			main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd, \
					main->cmd_info->cmd_path,
					main->cmds_paths->paths[i++]);
			if (access(main->cmd_info->cmd_path, X_OK) == 0 \
				&& ft_strlen(main->cmd_info->cmd) != 0)
			{
				cmd_found = llaunch_executions(main, envp, tube, -1);
				break ;
			}
			handle_null_case(main, envp, tube, i);
			free_cmd_path(main);
		}
	}
}

static int	launch_executions(t_main *main, char **envp, int file, int i)
{
	if (i == -2)
	{
		free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
	}
	main->tube->fd = cmd_executor(main, envp, file, i);
	free(main->cmd_info->cmd_path);
	main->cmd_info->cmd_path = NULL;
	return (1);
}

int	cmd_searcher(t_main *main, char **envp, int file)
{
	auto int i = 0;
	cmds_paths_maker(main);
	if (ft_strchr(main->cmd_info->cmd, '/') || !main->cmds_paths \
		|| !main->cmds_paths->paths || !main->cmds_paths->paths[0]
		|| ft_strchr(main->cmd_info->cmd, '.'))
		relative_path_executor(main, envp, 0);
	else
	{
		while (main->cmds_paths->paths && main->cmds_paths->paths[i])
		{
			main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
					main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
			if (access(main->cmd_info->cmd_path, X_OK) == 0 \
				&& ft_strlen(main->cmd_info->cmd) != 0)
				return (launch_executions(main, envp, file, i), main->tube->fd);
			ft_strdup_cmd_path(main, i);
			if (!main->cmds_paths->paths[i] && main->cmd_info->cmd[0] != '/')
				main->tube->fd = cmd_executor(main, envp, file, i);
			free_cmd_path(main);
		}
	}
	return (main->tube->fd);
}
