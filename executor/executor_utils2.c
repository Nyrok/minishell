/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_heredoc(char *end)
{
	char	*line;
	int		tube[2];
	int		fd;
	int		fd_return;

	line = readline("> ");
	if (pipe(tube) == -1)
	{
		perror("pipe");
		return (-1);
	}
	fd = tube[1];
	fd_return = tube[0];
	while (line != NULL && ft_strcmp(line, end) != 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	free(line);
	close(fd);
	return (fd_return);
}

int	no_such_file(t_redir *actual_redir)
{
	printf("-minishell: %s: No such file or directory\n",
		actual_redir->filename);
	return (-1);
}

int	permission_denied(t_main **main, t_redir *actual_redir)
{
	if (actual_redir->type == REDOUT
		&& access(actual_redir->filename, F_OK) != 0)
	{
		actual_redir->fd = open(actual_redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		return (0);
	}
	printf("-minishell: %s: Permission denied\n", actual_redir->filename);
	(*main)->last_exit_status = 127;
	return (-1);
}

int	fd_opener(t_main **main, t_redir *actual_redir)
{
	if (actual_redir->type == REDIN \
		&& access(actual_redir->filename, F_OK) != 0)
		return (no_such_file(actual_redir));
	else if (access(actual_redir->filename, R_OK) != 0 && \
			actual_redir->type != HEREDOC)
		return (permission_denied(main, actual_redir));
	if (actual_redir->type == APPEND)
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (actual_redir->type == REDIN)
		actual_redir->fd = open(actual_redir->filename, O_RDONLY, 0444);
	else if (actual_redir->type == HEREDOC)
		actual_redir->fd = ft_heredoc(actual_redir->filename);
	else
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (actual_redir->fd == -1)
		return (-1);
	if (actual_redir->good == 0)
		close(actual_redir->fd);
	return (1);
}

void	multiple_cmd_handler(t_main *main, char **envp,
			pid_t **pids, int nbcmds)
{
	t_cmd_info	*temp_cmd_info;

	if (nbcmds == 0)
		free_cmd_info(&main->cmd_info);
	while (nbcmds > 0 && main->cmd_info)
	{
		setup_cmd_redirs(main->cmd_info);
		if (main->tube == NULL)
		{
			*pids = ft_calloc((count_cmd_info(main->cmd_info) + 1), \
				sizeof(pid_t));
			setup_tube(main);
		}
		multiplecmdexector(main, envp, pids, nbcmds--);
		temp_cmd_info = main->cmd_info;
		main->cmd_info = main->cmd_info->next;
		free_cmd_info(&temp_cmd_info);
	}
	main->cmd_info = NULL;
}
