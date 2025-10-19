/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_leaks(t_main *main)
{
	t_cmd_info		*free_tmp;

	free(main->str_envp);
	main->str_envp = NULL;
	if (main->tube && main->tube->fd != -1)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
	if (main->tube)
		free(main->tube);
	main->tube = NULL;
	while (main->cmd_info)
	{
		free_tmp = main->cmd_info;
		main->cmd_info = main->cmd_info->next;
		free(free_tmp);
		free_tmp = NULL;
	}
	return (1);
}

int	hasinfile(struct s_main **main, int error_check, int *print)
{
	t_cmd_info	*actual_cmd;
	t_redir		*actual_redir;
	int			total;
	int			has_infile;

	total = 0;
	actual_cmd = (*main)->cmd_info;
	while (actual_cmd != NULL)
	{
		actual_redir = actual_cmd->redirs;
		while (actual_redir != NULL)
		{
			has_infile = fd_opener(main, actual_redir, error_check, *print);
			if (has_infile == -1)
				return (-1);
			if (has_infile == -2)
				return (-2);
			if (actual_redir->type == 3)
				total = 1;
			actual_redir = actual_redir->next;
		}
		actual_cmd = actual_cmd->next;
	}
	*print = 0;
	return (total);
}

void	setup_tube(t_main *main)
{
	main->tube = ft_calloc(1, sizeof(t_redir));
	main->tube->next = NULL;
	main->tube->fd = -1;
}
