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
				redir->fd = ft_heredoc(redir->filename);
				if (redir->fd == -1)
					return (-1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
