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

int	file_executor(t_main *main, int file, int last)
{
	char	**tmp;
	int		i;

	i = 0;
	isfilevalid(main);
	tmp = ft_split(main->cmd_info->cmd, ' '); // Voir si ca passe ici les commandes genre "./cc dd"
	main->cmd_info->cmd_path = ft_strdup(tmp[0]);
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
	if (last == 0)
		main->tube->fd = cmd_executor(main, main->str_envp, file, -1); // ok enft je ne charge jamais le tube ou le infile
	else
	{
		if (main->cmd_info->infile)
			last_executor(main, main->str_envp, main->cmd_info->infile->fd, -1);
		else
			last_executor(main, main->str_envp, main->tube->fd, -1); // ok enft je ne charge jamais le tube ou le infile
	}
	free(main->cmd_info->cmd_path);
	main->cmd_info->cmd_path = NULL;
	return (1);
}

void	ft_dup2(int src, int dest)
{
	if (dup2(src, dest) == -1)
		perror("dup2 failed");
}

void	close_heredoc_future_cmds(t_main *main)
{
	t_cmd_info	*tmp_cmd_info;
	t_redir		*tmp_redir;

	if (main->cmd_info && main->cmd_info->next)
	{
		tmp_cmd_info = main->cmd_info->next; // s'assurer du bon fonctionnement de cette fonction
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
		close(file);
		file = -1;
	}
	if (tube[1] != -1)
	{
		printf("ccc\n");
		ft_dup2(tube[1], STDOUT_FILENO);
		close(tube[1]);
		tube[1] = -1;
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

int	cmd_executor(t_main *main, char **envp, int file, int i)
{
	pid_t	pid;
	int		tube[2];

	if (pipe(tube) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if ((i == -2 && check_if_exist(main) == 0) || (i > 0 && main->cmds_paths->paths[i] == NULL)) // pas sûr de la condition i != -1 -> i > 0
	{
		//printf("CC\n");
		print_error(main, NOTFOUND, 0);
		close(tube[1]);
		tube[1] = -1;
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
	printf("Exit status : %d\n", main->last_exit_status);
	return (1);
}
