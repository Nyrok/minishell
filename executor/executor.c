/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_executor(t_main *main, int file, pid_t **pids, int last)
{
	char	**tmp;
	int		i;
	char	**envp;

	i = 0;
	tmp = ft_split(main->cmd_info->cmd, ' ');
	envp = envp_to_str(main->envp);
	main->cmd_info->cmd_path = ft_strdup(tmp[0]);
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
	if (last == 0)
		main->tube->fd = cmd_executor(main, envp, file, pids);
	else
	{
		last_executor(main, envp, main->tube->fd, pids);
		main->tube->fd = -1;
	}
	free(main->cmd_info->cmd_path);
	return (1);
}

int	ft_access(char *pathname)
{
	if (access(pathname, X_OK) != 0)
	{
		printf("-minishell: %s: Permission denied", pathname);
		return (0);
	}
	return (1);
}

void	child_executor(t_main *main, int *tube, int file, char **envp)
{
	close(tube[0]);
	if (file != -1)
		dup2(file, STDIN_FILENO);
	dup2(tube[1], STDOUT_FILENO);
	close(tube[1]);
	if (file != -1)
		close(file);
	if (execve(main->cmd_info->cmd_path,
			(char *const *)main->cmd_info->argv, envp) == -1)
	{
		close(tube[1]);
		close(file);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	(void)main->cmd_info->argv;
}

int	cmd_executor(t_main *main, char **envp, int file, pid_t **pids)
{
	pid_t	pid;
	int		tube[2];

	if (pipe(tube) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		child_executor(main, tube, file, envp);
	else
	{
		close(tube[1]);
		if (file != -1)
			close(file);
		add_pid(pids, pid);
	}
	return (tube[0]);
}

int	executor(char *cmd, struct s_main *main)
{
	pid_t		*pids;
	int			nbcmds;
	char		**envp;
	t_cmd_info	*temp_cmd_info;

	pids = malloc((totalcmds(cmd) + 1) * sizeof(pid_t));
	if (!pids)
		return (0);
	envp = envp_to_str(main->envp);
	executor_setup(main, pids, &nbcmds, cmd);
	if (main->cmd_info->cmd == NULL)
		return (0);
	if (nbcmds == 1)
	{
		onecmdexector(main, envp, &pids);
		temp_cmd_info = main->cmd_info;
		free_cmd_info(&temp_cmd_info);
		main->cmd_info = NULL;
	}
	else
		multiple_cmd_handler(main, envp, &pids, nbcmds);
	if (pids)
		end_pids(&pids);
	no_leaks(main, envp);
	return (1);
}
