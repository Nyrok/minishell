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

void	add_pid(pid_t **pids, pid_t newpid)
{
	int		i;
	pid_t	*actual;

	i = 0;
	actual = *pids;
	while (actual[i] != 0)
		i++;
	actual[i] = newpid;
	actual[i + 1] = 0;
}

void	end_pids(pid_t **pids)
{
	int		i;
	int		status;
	pid_t	*actual;

	i = 0;
	actual = *pids;
	while (actual[i] != 0)
	{
		waitpid(actual[i], &status, 0);
		i++;
	}
	free(actual);
}

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

void	lcmd_searcher(t_main *main, char **envp, int tube, pid_t **pids)
{
	int			cmdopener;

	auto int i = 0;
	auto int cmd_found = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
		cmd_found = file_executor(main, -1, pids, 1);
	while (main->cmds_paths->paths[i])
	{
		main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
				main->cmd_info->cmd_path, main->cmds_paths->paths[i]);
		cmdopener = open(main->cmd_info->cmd_path, O_RDONLY);
		if (cmdopener != -1)
		{
			close(cmdopener);
			if (ft_access(main->cmd_info->cmd_path) == 0)
				break ;
			cmd_found = 1;
			last_executor(main, envp, tube, pids);
			free(main->cmd_info->cmd_path);
			break ;
		}
		free(main->cmd_info->cmd_path);
		i++;
	}
	print_error(main, NOTFOUND, cmd_found);
}

int	cmd_searcher(t_main *main, char **envp, int file, pid_t **pids)
{
	int		cmdopener;

	auto int i = 0;
	auto int cmd_found = 0;
	if (main->cmd_info->cmd[0] == '.' && main->cmd_info->cmd[1] == '/')
		cmd_found = file_executor(main, file, pids, 0);
	while (main->cmds_paths->paths[i])
	{
		main->cmd_info->cmd_path = paths_searcher(main->cmd_info->cmd,
				main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
		cmdopener = open(main->cmd_info->cmd_path, O_RDONLY);
		if (cmdopener != -1)
		{
			if (ft_access(main->cmd_info->cmd_path) == 0)
				break ;
			close(cmdopener);
			cmd_found = 1;
			main->tube->fd = cmd_executor(main, envp, file, pids);
			free(main->cmd_info->cmd_path);
			break ;
		}
		free(main->cmd_info->cmd_path);
	}
	print_error(main, NOTFOUND, cmd_found);
	return (main->tube->fd);
}
