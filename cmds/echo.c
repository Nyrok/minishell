/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_main *main, int argc, const char **argv, int nbcmds)
{
	int	i;
	int	nl;
	int	tube[2];
	int	fd;

	fd = STDOUT_FILENO;
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
	i = 1;
	if (argc > 1 && ft_strncmp(argv[1], "-n", 2) == 0 && i++)
		nl = 1;
	while (i < argc)
	{
		write(fd, argv[i], ft_strlen(argv[i]));
		if (++i < argc)
			write(fd, " ", 1);
	}
	if (argc == 1 || nl)
		write(fd, "\n", 1);
	if (main->cmd_info->outfile != NULL)
		close(main->cmd_info->outfile->fd);
	else if (nbcmds > 1)
		close(tube[1]);
	return (1);
}
