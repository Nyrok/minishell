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

int	no_leaks(t_main *main, char **envp)
{
	t_cmd_info		*free_tmp;

	free(envp);
	if (main->tube && main->tube->fd != -1)
		close(main->tube->fd);
	if (main->tube)
		free(main->tube);
	while (main->cmd_info)
	{
		free_tmp = main->cmd_info;
		main->cmd_info = main->cmd_info->next;
		free(free_tmp);
		free_tmp = NULL;
	}
	free_tokens(&main->tokens);
	return (1);
}

int	hasinfile(struct s_main *main)
{
	t_cmd_info	*actual_cmd;
	t_redir		*actual_redir;
	int			total;

	total = 0;
	actual_cmd = main->cmd_info;
	while (actual_cmd != NULL)
	{
		actual_redir = actual_cmd->redirs;
		while (actual_redir != NULL)
		{
			if (fd_opener(actual_redir) == -1)
				return (-1);
			if (actual_redir->type == 3)
				total = 1;
			actual_redir = actual_redir->next;
		}
		actual_cmd = actual_cmd->next;
	}
	return (total);
}

int	hasoutfile(struct s_main *main)
{
	t_redir	*actual;
	int		total;

	total = 0;
	actual = main->cmd_info->redirs;
	while (actual != NULL)
	{
		printf("Enter\n");
		if (actual->type == 5)
			total = 1;
		actual = actual->next;
	}
	return (total);
}

void	setup_tube(t_main *main)
{
	main->tube = ft_calloc(1, sizeof(t_redir));
	main->tube->next = NULL;
	main->tube->fd = -1;
}

void	reset_tube(t_main *main)
{
	if (main->tube != NULL && main->tube->fd >= 0)
		close(main->tube->fd);
	main->tube = NULL;
}
