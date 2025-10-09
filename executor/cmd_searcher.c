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
	//char	**res;
	// int		i;

	tmp = ft_strjoin(paths, "/");
	// i = 0;
	cmd_path = ft_strjoin(tmp, cmd);
	//printf("CMD Path = %s\n", cmd_path);
	free(tmp);
	//res = ft_split(cmd_path, ' ');
	//printf("CMD Path apres split = %s | %s\n", res[0], res[1]);
	//free(cmd_path);
	//cmd_path = ft_strdup(res[0]);
	//while (res[i])
	//{
	//	free(res[i]);
	//	i++;
	//}
	//free(res);
	return (cmd_path);
}

int	llaunch_executions(t_main *main, char **envp, int tube, int i)
{
	if (i == -2)
	{
		free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
		// if (access(main->cmd_info->cmd_path, F_OK) == 0)
		// 	i = -1;
	}
	last_executor(main, envp, tube, i);
	free(main->cmd_info->cmd_path);
	main->cmd_info->cmd_path = NULL;
	return (1);
}

void	handle_null_case(t_main *main, char **envp, int tube, int i)
{
	//if (main->cmds_paths->paths[i] == NULL && main->cmd_info && ft_strlen(main->cmd_info->cmd) == 0)
	//{
	//	printf("CC1\n");
	//	printf("%s: command not found\n", main->cmd_info->cmd);
	//	main->last_exit_status = 127;
	//	main->tube->fd = create_eof_fd(main); // Voir si dans tt les cas main->tube existe bien
	//	printf("xx = %d\n", main->tube->fd);
	//}
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info && ft_strlen(main->cmd_info->cmd) == 0)
	{
		free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
		main->last_exit_status = 127;
	}
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] != '/')
		last_executor(main, envp, tube, i);
	if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] == '/')
		llaunch_executions(main, envp, tube, -2);
}

int	gestion_error_relative_path(t_main *main, int type)
{
	if (type == 1)
	{
		if (access(main->cmd_info->cmd, F_OK) != 0) // Check si ici on doit pas free des trucs genre infile, main tube etc.
		{
			printf("minishell: %s: No such file or directory\n", main->cmd_info->cmd);
			main->last_exit_status = 127;
			fork_bad_file(main);
			return (1);
		}
		else if (access(main->cmd_info->cmd, X_OK) != 0)
		{
			printf("minishell: %s: Permission denied\n", main->cmd_info->cmd);
			main->last_exit_status = 126;
			fork_bad_file(main);
			return (1);
		}
	}
	else
	{
		if (access(main->cmd_info->cmd_path, F_OK) != 0) // Check si ici on doit pas free des trucs genre infile, main tube etc.
		{
			printf("minishell: %s: No such file or directory\n", main->cmd_info->cmd);
			main->last_exit_status = 127;
			fork_bad_file(main);
			return (1);
		}
		else if (access(main->cmd_info->cmd_path, X_OK) != 0)
		{
			printf("minishell: %s: Permission denied\n", main->cmd_info->cmd);
			main->last_exit_status = 126;
			fork_bad_file(main);
			return (1);
		}
	}
	return (0);
}

void	relative_path_executor(t_main *main, char **envp, int lastcmd)
{
	auto int error = 0;
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
	if (main->cmd_info->cmd_path)
		free(main->cmd_info->cmd_path);
	//printf("strchr = %s\n", ft_strchr(main->cmd_info->cmd, '/'));
	if (!ft_strchr(main->cmd_info->cmd, '/') && (!main->cmds_paths || !main->cmds_paths->paths))
	{
		//printf("PUTTER\n");
		auto char *tmp = ft_strdup("./");
		//printf("C = %s\n", tmp);
		main->cmd_info->cmd_path = ft_strjoin(tmp, main->cmd_info->cmd);
		if (tmp)
			free(tmp);
		//printf("CC = %s\n", main->cmd_info->cmd_path);
		error = gestion_error_relative_path(main, 1);
	}
	else
	{
		main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
		error = gestion_error_relative_path(main, 1);
	}
	if (error == 1)
	{
		free(main->cmd_info->cmd_path);
		main->cmd_info->cmd_path = NULL;
		if (main->cmds_paths)
		{
			free(main->cmds_paths);
			main->cmds_paths = NULL;
		}
		return ;
	}
	if (lastcmd == 0)
	{
		if (main->cmd_info->infile)
		{
			//printf("ALLO\n");
			main->tube->fd = cmd_executor(main, envp, main->cmd_info->infile->fd, -1);
		}
		else
			main->tube->fd = cmd_executor(main, envp, main->tube->fd, -1);
	}
	else
	{
		if (main->cmd_info->infile)
			last_executor(main, envp, main->cmd_info->infile->fd, -1);
		else
		{
			//printf("cc = %s, %s\n", main->cmd_info->cmd, main->cmd_info->cmd_path);
			last_executor(main, envp, main->tube->fd, -1);
		}
	}
	free(main->cmd_info->cmd_path);
	main->cmd_info->cmd_path = NULL;
	if (main->cmds_paths)
	{
		free(main->cmds_paths);
		main->cmds_paths = NULL;
	}
}

void	lcmd_searcher(t_main *main, char **envp, int tube)
{
	auto int i = 0;
	auto int cmd_found = 0;
	//if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
	//	cmd_found = file_executor(main, -1, 1);
	cmds_paths_maker(main);
	if (ft_strchr(main->cmd_info->cmd, '/') || !main->cmds_paths || !main->cmds_paths->paths || !main->cmds_paths->paths[0])
		relative_path_executor(main, envp, 1);
	else
	{
		while (main->cmds_paths->paths && main->cmds_paths->paths[i]
			&& cmd_found != -1)
		{
			main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
					main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);		
			if (access(main->cmd_info->cmd_path, X_OK) == 0 && ft_strlen(main->cmd_info->cmd) != 0)
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
	//if (main->cmd_info->cmd_path)
	//	free(main->cmd_info->cmd_path);
	//main->cmd_info->cmd_path = NULL;
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

int	cmd_searcher(t_main *main, char **envp, int file)
{
	auto int i = 0;
	cmds_paths_maker(main); // mis avant le if
	if (ft_strchr(main->cmd_info->cmd, '/') || !main->cmds_paths || !main->cmds_paths->paths || !main->cmds_paths->paths[0])
		relative_path_executor(main, envp, 0);
	else
	{
		while (main->cmds_paths->paths && main->cmds_paths->paths[i])
		{
			main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
					main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
			if (access(main->cmd_info->cmd_path, X_OK) == 0 && ft_strlen(main->cmd_info->cmd) != 0)
			{
				launch_executions(main, envp, file, i);
				break ;
			}
			if (main->cmds_paths->paths[i] == NULL && main->cmd_info && ft_strlen(main->cmd_info->cmd) == 0)
			{
				free(main->cmd_info->cmd_path);
				main->cmd_info->cmd_path = ft_strdup(main->cmd_info->cmd);
				main->last_exit_status = 127;
			}
			if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] != '/')
				main->tube->fd = cmd_executor(main, envp, file, i);
			if (main->cmds_paths->paths[i] == NULL && main->cmd_info->cmd[0] == '/')
				launch_executions(main, envp, file, -2);
			if (main->cmd_info->cmd_path)
				free(main->cmd_info->cmd_path);
			main->cmd_info->cmd_path = NULL; // ajt
		}
	}
	//if (main->cmd_info->cmd_path)
	//	free(main->cmd_info->cmd_path);
	//main->cmd_info->cmd_path = NULL;
	return (main->tube->fd);
}
