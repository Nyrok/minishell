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

void	print_env(int fd, t_envp *actual)
{
	while (actual != NULL)
	{
		write(fd, actual->data, ft_strlen(actual->data));
		write(fd, "\n", 1);
		actual = actual->next;
	}
}

int	env(t_main *main, t_envp *datas, int nbcmds)
{
	t_envp	*actual;
	int		tube[2];
	int		fd;

	if (main->cmd_info->outfile != NULL)
		fd = main->cmd_info->outfile->fd;
	else if (nbcmds > 1)
	{
		if (pipe(tube) == -1)
		{
			perror("pipe");
			return (-1);
		}
		fd = tube[1];
		main->tube->fd = tube[0];
	}
	else
		fd = STDOUT_FILENO;
	actual = datas;
	print_env(fd, actual);
	if (main->cmd_info->outfile != NULL)
		close(main->cmd_info->outfile->fd);
	else if (nbcmds > 1)
		close(tube[1]);
	return (1);
}
