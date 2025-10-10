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

	tmp = ft_strjoin(paths, "/");
	cmd_path = ft_strjoin(tmp, cmd);
	free(tmp);
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
	ft_strdup_cmd_path(main, i); //norme
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] != '/')
		last_executor(main, envp, tube, i);
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] == '/')
		llaunch_executions(main, envp, tube, -2);
}

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
	if (main->cmds_paths && main->cmds_paths->paths && !main->cmds_paths->paths[0])
	{
		free(main->cmds_paths->paths);
		main->cmds_paths->paths = NULL;
	}
	else if (main->cmds_paths && main->cmds_paths->paths && main->cmds_paths->paths[0])
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
	free_cmd_path(main); // le met a null jsp si ici c bien a voir
	//error = error_no_path(main, error);
	error = error_no_path(main, error);
	if (error == 1)
		return (free_cmd_path(main), free_main_paths(main)); // norme
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

int	launch_executions(t_main *main, char **envp, int file, int i)
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

void	ft_strdup_cmd_path(t_main *main, int i)
{
	if (!main->cmds_paths->paths[i] && main->cmd_info \
		&& ft_strlen(main->cmd_info->cmd) == 0)
	{
		free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
		main->last_exit_status = 127;
	}
}

int	cmd_searcher(t_main *main, char **envp, int file)
{
	auto int i = 0;
	cmds_paths_maker(main);
	if (ft_strchr(main->cmd_info->cmd, '/') || !main->cmds_paths \
		|| !main->cmds_paths->paths || !main->cmds_paths->paths[0])
		relative_path_executor(main, envp, 0);
	else
	{
		while (main->cmds_paths->paths && main->cmds_paths->paths[i])
		{
			main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
					main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
			if (access(main->cmd_info->cmd_path, X_OK) == 0 \
				&& ft_strlen(main->cmd_info->cmd) != 0)
				return (launch_executions(main, envp, file, i), main->tube->fd); // au lieu du break, on gagne des lignes
			ft_strdup_cmd_path(main, i); // norme
			if (!main->cmds_paths->paths[i] && main->cmd_info->cmd[0] != '/') // bzr condition on rentre dans les 2 if
				main->tube->fd = cmd_executor(main, envp, file, i);
			free_cmd_path(main);
		}
	}
	return (main->tube->fd);
}
