/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	openoutfile(t_main *main)
{
	if (main->cmd_info->outfile->type == REDOUT)
	{
		main->cmd_info->outfile->fd = open(main->cmd_info->outfile->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (main->cmd_info->outfile->type == APPEND)
	{
		main->cmd_info->outfile->fd = open(main->cmd_info->outfile->filename,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
}

int	check_outfile(t_main *main, int *fd, int nbcmds)
{
	if (main->cmd_info->outfile)
	{
		if (access(main->cmd_info->outfile->filename, F_OK) != 0)
		{
			openoutfile(main);
			*fd = main->cmd_info->outfile->fd;
			if (nbcmds > 1)
				delete_tube(main);
		}
		else if (access(main->cmd_info->outfile->filename, W_OK) == 0)
		{
			openoutfile(main);
			*fd = main->cmd_info->outfile->fd;
			if (nbcmds > 1)
				delete_tube(main);
		}
		else
			return (perror("Permission denied"), 2);
	}
	return (1);
}

void	print_env(t_main *main, int fd, t_envp *actual, int nbcmds)
{
	if (check_outfile(main, &fd, nbcmds) == 2)
		return ;
	while (actual != NULL)
	{
		write(fd, actual->full, ft_strlen(actual->full));
		write(fd, "\n", 1);
		actual = actual->next;
	}
}

int	env(t_main *main, t_envp *envp, int onlyonecommand, int nbcmds)
{
	t_envp	*actual;

	actual = envp;
	(void)onlyonecommand;
	print_env(main, STDOUT_FILENO, actual, nbcmds);
	return (1);
}
