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

static size_t	print_echo_args(int fd, int argc, const char **argv, int start)
{
	int		i;
	size_t	len;

	i = start;
	len = 0;
	while (i < argc)
	{
		len += write(fd, argv[i], ft_strlen(argv[i]));
		if (++i < argc)
			len += write(fd, " ", 1);
	}
	return (len);
}

static void	close_fd_if_needed(t_main *main, int nbcmds, int fd)
{
	if (main->cmd_info->outfile != NULL && main->cmd_info->outfile->fd != -1)
	{
		close(main->cmd_info->outfile->fd);
		main->cmd_info->outfile->fd = -1;
	}
	(void)fd;
	(void)nbcmds;
}

int	echo(t_main *main, int argc, const char **argv, int nbcmds)
{
	int		fd;
	int		nl;
	int		i;
	size_t	len_writed;

	fd = STDOUT_FILENO;
	if (check_outfile(main, &fd, nbcmds) == 2)
		return (1);
	i = 1;
	nl = is_n_flag(argc, argv, &i);
	len_writed = print_echo_args(fd, argc, argv, i);
	if (argc == 1 || nl || !len_writed)
		write(fd, "\n", 1);
	close_fd_if_needed(main, nbcmds, fd);
	return (1);
}
