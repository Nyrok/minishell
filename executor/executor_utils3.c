/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tube_handler(t_main **main)
{
	printf("zz\n");
	if ((*main)->cmd_info->infile != NULL)
	{
		if (fd_opener(main, (*main)->cmd_info->infile, 0) == -1)
			return (-1);
		if ((*main)->tube->fd != -1)
		{
			close((*main)->tube->fd);
			(*main)->tube->fd = -1;
		}
	}
	return (1);
}

int	print_error(t_main *main, int error_code, int cmd_found)
{
	if (error_code == NOTFOUND && cmd_found == 0)
	{
		printf("%s: command not found\n", main->cmd_info->cmd);
		main->last_exit_status = 127;
		main->tube->fd = -1;
	}
	else if (error_code == DEF_PIPE && cmd_found == 0)
	{
		perror("pipe"); // Pas sur de ca un printf perso serait peut etre mieux
		return (-1);
	}
	return (1);
}

int	totalcmds(char *cmd)
{
	int	i;
	int	total;

	i = 0;
	total = 1;
	while (cmd[i])
	{
		if (cmd[i] == '|')
			total++;
		i++;
	}
	return (total);
}
