/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_access(t_main *main, int j)
{
	if (j == 0)
	{
		printf("minishell: ./: Is a file\n");
		main->last_exit_status = 127;
		return (-1);
	}
	return (1);
}

int	isfilevalid(t_main *main)
{
	char	*filename;

	auto int i = 0;
	while (main->cmd_info->cmd[i] && main->cmd_info->cmd[i + 1])
	{
		if (main->cmd_info->cmd[i] != '.' || main->cmd_info->cmd[i + 1] != '/')
			break ;
		i += 2;
	}
	auto int j = 0;
	while (main->cmd_info->cmd[i + j])
		j++;
	if (j == 0)
		return (check_access(main, j));
	filename = malloc((j + 1) * sizeof(char));
	j = 0;
	while (main->cmd_info->cmd[i + j])
	{
		filename[j] = main->cmd_info->cmd[i + j];
		j++;
	}
	filename[j] = '\0';
	if (access(filename, F_OK | X_OK) == 0)
		return (free(filename), -1);
	return (free(filename), 1);
}

int	fdcls(t_main **main, int error)
{
	t_cmd_info	*cmd_tmp;
	t_cmd_info	*to_free;
	t_redir		*actual_redir;

	cmd_tmp = (*main)->cmd_info;
	while (cmd_tmp)
	{
		to_free = cmd_tmp;
		actual_redir = to_free->redirs;
		while (actual_redir != NULL)
		{
			if (actual_redir->fd != -1)
				close(actual_redir->fd);
			actual_redir = actual_redir->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	printf("aaa\n");
	if (error == 1)
		(*main)->cmd_info = NULL;
	return (1);
}
