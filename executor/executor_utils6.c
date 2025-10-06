/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_exist(t_main *main)
{
	int		i;
	int		cmd_found;
	char	*tmp;

	i = 0;
	cmd_found = 0;
	while (main->cmds_paths->paths && main->cmds_paths->paths[i]
		&& main->cmd_info->cmd[0] != '.')
	{
		tmp = paths_searcher(main->cmd_info->cmd,
				main->cmd_info->cmd_path, main->cmds_paths->paths[i++]);
		if (access(tmp, F_OK) == 0)
		{
			cmd_found = 1;
			break ;
		}
		free(tmp);
		tmp = NULL;
	}
	if (access(main->cmd_info->cmd, F_OK) == 0)
		cmd_found = 1;
	if (tmp)
		free(tmp);
	return (cmd_found);
}
