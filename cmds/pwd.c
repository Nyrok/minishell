/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_main *main, int nbcmds)
{
	int		tube[2];
	int		fd;
	char	*str;

	str = getenv("PWD");
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
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
	if (main->cmd_info->outfile != NULL)
		close(main->cmd_info->outfile->fd);
	else if (nbcmds > 1)
		close(tube[1]);
	return (1);
}
