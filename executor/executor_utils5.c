/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_bad_file(t_main *main)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid == 0)
	{
		exit_code = main->last_exit_status;
		free_execve(&main);
		exit(exit_code);
	}
	else
		add_pid(main, pid);
	main->tube->fd = create_eof_fd(main);
}

void	ft_dup2(int src, int dest)
{
	if (dup2(src, dest) == -1)
		perror("dup2 failed");
}

char	*paths_searcher(char *cmd, char *cmd_path, char *paths)
{
	char	*tmp;

	tmp = ft_strjoin(paths, "/");
	cmd_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (cmd_path);
}

int	hasinfile2(struct s_main **main, int error_check, int print)
{
	t_redir		*actual_redir;
	int			total;
	int			has_infile;

	total = 0;
	actual_redir = (*main)->cmd_info->redirs;
	(void)error_check;
	while (actual_redir != NULL)
	{
		has_infile = fd_opener(main, actual_redir, -1, print);
		if (has_infile == -1)
			return (-1);
		if (has_infile == -2)
			return (-2);
		if (actual_redir->type == 3)
			total = 1;
		actual_redir = actual_redir->next;
	}
	return (total);
}

int	handle_heredoc(t_main *main)
{
	t_redir	*redir_tmp;
	int		fd;

	redir_tmp = main->cmd_info->redirs;
	while (redir_tmp)
	{
		if (redir_tmp->type == HEREDOC)
		{
			fd = ft_heredoc(main, redir_tmp->filename);
			if (fd != -1)
			{
				close(fd);
				return (-1);
			}
		}
		redir_tmp = redir_tmp->next;
	}
	return (-1);
}
