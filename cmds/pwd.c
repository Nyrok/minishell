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
	int		fd;
	char	*str;

	(void)nbcmds;
	str = get_env_value(main->envp, "PWD");
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
	return (1);
}
