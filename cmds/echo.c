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

static int	is_n_flag(int argc, const char **argv, int *i)
{
	if (argc > 1 && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		(*i)++;
		return (1);
	}
	return (0);
}

static void	print_echo_args(int fd, int argc, const char **argv, int start)
{
	int	i;

	i = start;
	while (i < argc)
	{
		write(fd, argv[i], ft_strlen(argv[i]));
		if (++i < argc)
			write(fd, " ", 1);
	}
}

static void	close_fd_if_needed(t_main *main, int nbcmds, int fd)
{
	if (main->cmd_info->outfile != NULL)
		close(main->cmd_info->outfile->fd);
	(void)fd;
	(void)nbcmds;
	//else if (nbcmds > 1)
	//	close(fd);
}

int	echo(t_main *main, int argc, const char **argv, int nbcmds)
{
	int	fd;
	int	nl;
	int	i;

	fd = STDOUT_FILENO;
	if (check_outfile(main, &fd, nbcmds) == 2)
		return (1);
	i = 1;
	nl = is_n_flag(argc, argv, &i);
	print_echo_args(fd, argc, argv, i);
	if (argc == 1 || nl)
		write(fd, "\n", 1);
	close_fd_if_needed(main, nbcmds, fd);
	return (1);
}
