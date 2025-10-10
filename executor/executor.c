/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_future_cmds(t_main *main)
{
	t_cmd_info	*tmp_cmd_info;
	t_redir		*tmp_redir;

	if (main->cmd_info && main->cmd_info->next)
	{
		tmp_cmd_info = main->cmd_info->next;
		while (tmp_cmd_info)
		{
			tmp_redir = tmp_cmd_info->redirs;
			while (tmp_redir)
			{
				if (tmp_redir->type == HEREDOC && tmp_redir->fd != -1)
				{
					close(tmp_redir->fd);
					tmp_redir->fd = -1;
				}
				tmp_redir = tmp_redir->next;
			}
			tmp_cmd_info = tmp_cmd_info->next;
		}
	}
}

void	child_executor(t_main *main, int *tube, int file, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	close(tube[0]);
	close_heredoc_future_cmds(main);
	if (file != -1)
	{
		ft_dup2(file, STDIN_FILENO);
		end_fd(file);
	}
	if (tube[1] != -1)
	{
		ft_dup2(tube[1], STDOUT_FILENO);
		end_fd(tube[1]);
	}
	if (execve(main->cmd_info->cmd_path,
			(char *const *)main->cmd_info->argv, envp) == -1)
	{
		if (tube[1] != -1)
			close(tube[1]);
		if (file != -1)
			close(file);
		auto int exit_code = free_execve(&main);
		perror("execve failed");
		exit(exit_code);
	}
}

void	handle_error_cmd(t_main	*main, int tube[2])
{
	print_error(main, NOTFOUND, 0);
	delete_tube(main);
	close(tube[1]);
	main->tube->fd = tube[0];
	fork_bad_file(main);
	tube[1] = -1;
}

int	cmd_executor(t_main *main, char **envp, int file, int i)
{
	pid_t	pid;
	int		tube[2];

	if (pipe(tube) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (i == -2 || (i != -1 && main->cmds_paths->paths[i] == NULL))
	{
		handle_error_cmd(main, tube);
		return (main->tube->fd);
	}
	pid = fork();
	if (pid == 0)
		child_executor(main, tube, file, envp);
	else
	{
		if (tube[1] != -1)
			close(tube[1]);
		if (file != -1)
			close(file);
		add_pid(main, pid);
	}
	return (tube[0]);
}

int	executor(char *cmd, struct s_main *main)
{
	int	nbcmds;

	main->pids = malloc((totalcmds(cmd) + 1) * sizeof(pid_t));
	if (!main->pids)
		return (0);
	main->str_envp = envp_to_str(main->envp);
	if (executor_setup(&main, &nbcmds, cmd) == -1)
		return (free_cmd_info(&main->cmd_info), no_leaks(main),
			end_pids(&main), 0);
	if (nbcmds == 1)
	{
		if (onecmdexector(main, main->str_envp) == -1)
			return (-1);
		free_cmd_info(&main->cmd_info);
	}
	else
	{
		if (multiple_cmd_handler(main, main->str_envp, nbcmds) == -1)
			return (-1);
	}
	if (main->pids)
		end_pids(&main);
	no_leaks(main);
	//printf("Exit status : %d\n", main->last_exit_status);
	return (1);
}
