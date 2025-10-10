/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relative_paths.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isonlypt(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '.')
			return (0);
		i++;
	}
	return (1);
}

int	gestion_error_relative_path(t_main *main, int type)
{
	if (ft_commandisapoint(main) == 1)
		return (1);
	if (ft_commandonlypoints(main) == 1)
		return (1);
	if (type == 1)
	{
		if (ft_isadirectory(main, main->cmd_info->cmd) == 1)
			return (1);
		if (ft_exist(main, main->cmd_info->cmd) == 1)
			return (1);
		else if (ft_execperm(main, main->cmd_info->cmd) == 1)
			return (1);
	}
	else
	{
		if (ft_isadirectory(main, main->cmd_info->cmd_path) == 1)
			return (1);
		if (ft_exist(main, main->cmd_info->cmd_path) == 1)
			return (1);
		else if (ft_execperm(main, main->cmd_info->cmd_path) == 1)
			return (1);
	}
	return (0);
}

void	relative_path_free(t_main *main)
{
	if (main->cmds_paths && main->cmds_paths->paths
		&& !main->cmds_paths->paths[0])
	{
		free(main->cmds_paths->paths);
		main->cmds_paths->paths = NULL;
	}
	else if (main->cmds_paths && main->cmds_paths->paths
		&& main->cmds_paths->paths[0])
	{
		auto int i = 0;
		if (main->cmds_paths && main->cmds_paths->paths)
		{
			while (main->cmds_paths->paths[i])
				free(main->cmds_paths->paths[i++]);
			free(main->cmds_paths->paths);
			main->cmds_paths->paths = NULL;
		}
	}
}

int	error_no_path(t_main *main, int error)
{
	if (!ft_strchr(main->cmd_info->cmd, '/') && !isonlypt(main->cmd_info->cmd)
		&& (!main->cmds_paths || !main->cmds_paths->paths))
	{
		auto char *tmp = ft_strdup("./");
		main->cmd_info->cmd_path = ft_strjoin(tmp, main->cmd_info->cmd);
		if (tmp)
			free(tmp);
		error = gestion_error_relative_path(main, 1);
	}
	else
	{
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
		error = gestion_error_relative_path(main, 1);
	}
	return (error);
}

void	relative_path_executor(t_main *main, char **envp, int lastcmd)
{
	auto int error = 0;
	relative_path_free(main);
	free_cmd_path(main);
	error = error_no_path(main, error);
	if (error == 1)
		return (free_cmd_path(main), free_main_paths(main));
	if (lastcmd == 0)
	{
		if (main->cmd_info->infile)
			main->tube->fd = cmd_executor(main, envp,
					main->cmd_info->infile->fd, -1);
		else
			main->tube->fd = cmd_executor(main, envp, main->tube->fd, -1);
	}
	else
	{
		if (main->cmd_info->infile)
			last_executor(main, envp, main->cmd_info->infile->fd, -1);
		else
			last_executor(main, envp, main->tube->fd, -1);
	}
	free_cmd_path(main);
	free_main_paths(main);
}
