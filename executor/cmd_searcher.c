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

char	*paths_searcher(char *cmd, char *cmd_path, char *paths)
{
	char	*tmp;
	char	**res;
	int		i;

	tmp = ft_strjoin(paths, "/");
	i = 0;
	cmd_path = ft_strjoin(tmp, cmd);
	free(tmp);
	res = ft_split(cmd_path, ' ');
	free(cmd_path);
	cmd_path = ft_strdup(res[0]);
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (cmd_path);
}

int	llaunch_executions(t_main *main, char **envp, int tube, int i)
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

void	handle_null_case(t_main *main, char **envp, int tube, int i)
{
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] != '/')
		last_executor(main, envp, tube, i);
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] == '/')
		llaunch_executions(main, envp, tube, -2);
}

void	lcmd_searcher(t_main *main, char **envp, int tube)
{
	auto int i = 0;
	auto int cmd_found = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
		cmd_found = file_executor(main, -1, 1);
	cmds_paths_maker(main);
	while (main->cmds_paths->paths && main->cmds_paths->paths[i]
		&& cmd_found != -1 && main->cmd_info->cmd[0] != '.')
	{
		main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
				main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
		if (access(main->cmd_info->cmd_path, X_OK) == 0)
		{
			cmd_found = llaunch_executions(main, envp, tube, -1);
			break ;
		}
		handle_null_case(main, envp, tube, i);
		if (main->cmd_info->cmd_path)
			free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = NULL;
	}
}

int	cmd_searcher(t_main *main, char **envp, int file)
{
	auto int i = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
		file_executor(main, file, 0);
	cmds_paths_maker(main);
	while (main->cmds_paths->paths && main->cmds_paths->paths[i]
		&& main->cmd_info->cmd[0] != '.')
	{
		main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
				main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
		if (access(main->cmd_info->cmd_path, X_OK) == 0)
		{
			launch_executions(main, envp, file, i);
			break ;
		}
		if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] != '/')
			main->tube->fd = cmd_executor(main, envp, file, i);
		if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] == '/')
			launch_executions(main, envp, file, i);
		if (main->cmd_info->cmd_path)
			free(main->cmd_info->cmd_path);
	}
	return (main->tube->fd);
}
