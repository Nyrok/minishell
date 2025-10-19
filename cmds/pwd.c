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

void	error_pwd(t_main *main)
{
	main->last_exit_status = 1;
	printf("minishell: pwd: error retrieving current directory: getcwd: ");
	printf("cannot access this directories: No such file or directory\n");
}

int	pwd(t_main *main, int nbcmds)
{
	int		fd;
	char	*str;

	str = malloc(1024 * sizeof(char));
	if (!str)
		return (free(str), -1);
	(void)nbcmds;
	if (getcwd(str, 1024) == NULL)
		return (free(str), error_pwd(main), 1);
	fd = STDOUT_FILENO;
	if (check_outfile(main, &fd, nbcmds) == 2)
		return (1);
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
	if (main->cmd_info->outfile != NULL && main->cmd_info->outfile->fd != -1)
	{
		close(main->cmd_info->outfile->fd);
		main->cmd_info->outfile->fd = -1;
	}
	main->last_exit_status = 0;
	free(str);
	str = NULL;
	return (1);
}
