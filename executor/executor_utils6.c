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

void	cmd_null(t_main *main)
{
	if (main->cmd_info)
	{
		main->last_exit_status = 1;
		fork_bad_file(main);
		hasinfile2(&main, 0, 1);
		fdcls(&main, 0);
	}
}

int	hasinfile_heredocs_only(t_main *main)
{
	t_cmd_info	*cmd;
	t_redir		*redir;

	cmd = main->cmd_info;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				redir->fd = ft_heredoc(main, redir->filename);
				if (redir->fd == -1)
					return (-1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	close_redsirs_norme(t_redir *actual_redir)
{
	close(actual_redir->fd);
	actual_redir->fd = -1;
}

int	is_a_file_and_not_an_exec(t_main *main, char **envp, int lastcmd)
{
	(void)envp;
	(void)lastcmd;
	if (main->cmds_paths->paths && main->cmds_paths->paths[0]
		&& ft_strchr(main->cmd_info->cmd, '.')
		&& !ft_strchr(main->cmd_info->cmd, '/'))
	{
		printf("minishell: %s: command not found\n", main->cmd_info->cmd);
		main->last_exit_status = 127;
		fork_bad_file(main);
		return (1);
	}
	return (0);
}
