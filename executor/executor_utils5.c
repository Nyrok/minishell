/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	isnocommand(t_main *main, int file, int tube[2])
{
	print_error(main, NOTFOUND, 0);
	close(tube[1]);
	tube[1] = -1;
	if (tube[1] != -1)
		close(tube[1]);
	if (file != -1)
		close(file);
}

void	lisnocommand(t_main *main, int tube)
{
	print_error(main, NOTFOUND, 0);
	if (tube != -1)
		close(tube);
	if (main->cmd_info->outfile != NULL
		&& main->cmd_info->outfile->fd != -1)
	{
		close(main->cmd_info->outfile->fd);
		main->cmd_info->outfile->fd = -1;
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

int	create_eof_fd(t_main *main, int code)
{
	int	pipefd[2];

	(void)code;
	if (main->cmd_info && main->cmd_info->cmd == NULL
		&& main->tube && main->tube->fd != -1)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	close(pipefd[1]);
	return (pipefd[0]);
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
