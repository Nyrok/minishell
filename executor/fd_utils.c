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

int	check_access(t_main *main, int j, char *filename)
{
	if (j == 0)
	{
		printf("minishell: ./: Is a file\n");
		main->last_exit_status = 127;
		return (-1);
	}
	if (filename != NULL)
	{
		if (access(filename, F_OK) != 0)
		{
			printf("minishell: ./%s: No such file or directory\n", filename);
			main->last_exit_status = 127;
			return (-1);
		}
		else if (access(filename, F_OK | X_OK) != 0)
		{
			printf("minishell: ./%s: No such file or directory\n", filename);
			main->last_exit_status = 126;
			return (-1);
		}
	}
	return (1);
}

int	isfilevalid(t_main *main)
{
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
		return (check_access(main, j, NULL));
	auto char *filename = malloc((j + 1) * sizeof(char));
	if (!filename)
		return (-1);
	j = 0;
	while (main->cmd_info->cmd[i + j])
	{
		filename[j] = main->cmd_info->cmd[i + j];
		j++;
	}
	filename[j] = '\0';
	if (check_access(main, 1, filename) == -1)
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
			if (actual_redir->good != 1 && actual_redir->type == HEREDOC && actual_redir->fd != -1)
			{
				close(actual_redir->fd);
				actual_redir->fd = -1;
			}
			else if (actual_redir->type != HEREDOC && actual_redir->fd != -1)
			{
				close(actual_redir->fd);
				actual_redir->fd = -1;
			}
			actual_redir = actual_redir->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	if (error == 1)
		(*main)->cmd_info = NULL;
	return (1);
}

void	append_opener(t_redir *actual_redir)
{
	if (actual_redir->io == STDOUT_FILENO \
		&& access(actual_redir->filename, F_OK) != 0)
	{
		actual_redir->fd = open(actual_redir->filename, O_CREAT, 0777);
		if (actual_redir->fd != -1)
			close(actual_redir->fd);
	}
}
