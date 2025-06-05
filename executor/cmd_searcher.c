/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_searcher.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char	**envp_str(t_envp *datas)
{
	char	**res;
	t_envp	*actual;
	int		i;

	i = 0;
	actual = datas;
	while (actual != NULL)
	{
		i++;
		actual = actual->next;
	}
	res = malloc((i + 1) * sizeof(char *));
	if (res == NULL)
		return (0);
	actual = datas;
	i = 0;
	while (actual != NULL)
	{
		res[i] = ft_strdup(actual->data);
		actual = actual->next;
	}
	res[i] = NULL;
	return (res);
}

int	commande_executor(char *cmd_path, char* const* args, t_envp *datas)
{
	int pipe_fd[2];
	pid_t	pid;
	char	buffer[1000000];
	int		status;

	pipe(pipe_fd);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		if (execve(cmd_path, args, envp_str(datas)) == -1)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(pipe_fd[1]);
		ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
		if (bytes_read >= 0)
			buffer[bytes_read] = '\0';
		printf("%s\n", buffer);
		waitpid(pid, &status, 0);
		close(pipe_fd[0]);
	}
	return (1);
}

void	cmd_searcher(char *cmd, char **paths, t_envp *datas)
{
	int		i;
	char	*cmd_path;
	char	**args;
	
	i = 0;
	args = ft_split(cmd, ' ');
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], "/");		
		cmd_path = ft_strjoin(cmd_path, cmd);
		cmd_path = ft_split(cmd_path, ' ')[0];
		if (open(cmd_path, O_RDONLY) != -1)
		{
			commande_executor(cmd_path, args, datas);
			free(cmd_path);
			break ;
		}
		i++;
		free(cmd_path);
	}
}
