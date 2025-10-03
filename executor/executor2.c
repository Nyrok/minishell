/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                         :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	last_child_executor(int tube, t_main *main, char *cmd_path, char **envp)
{
	signal(SIGQUIT, SIG_DFL); // ?????????
	// if (tube == -1)
	// {
	// 	if (main->cmd_info->infile && fcntl(main->cmd_info->infile->fd, F_GETFD) != -1)
	// 		printf("Infile detected: %s\n", main->cmd_info->infile->filename);
	// 	else
	// 		printf("FD is not open or no redirection.\n");
	// 	if (dup2(main->tube->fd, STDIN_FILENO) == -1)
	// 		perror("dup2 failed");
	// 	main->tube->fd = -1;
	// }
	// close(main->tube->fd);
	if (main->cmd_info->tube[0] != -1 && tube != main->cmd_info->tube[0])
		close(main->cmd_info->tube[0]);
	if (tube != -1)
	{
		printf("TT = %d\n", tube);
		if (dup2(tube, STDIN_FILENO) == -1)
			perror("dup2 failed");
		close(tube);
		tube = -1;
	}
	printf("CC : %s\n", main->cmd_info->cmd_path);
	if (main->tube && main->tube->fd != -1)
		close(main->tube->fd);
	if (main->cmd_info->outfile != NULL)
	{
		fd_opener(&main, main->cmd_info->outfile, 0);
		if (fcntl(main->cmd_info->outfile->fd, F_GETFD) == -1)
		{
			perror("outfile fd is not open");
			exit(EXIT_FAILURE);
		}
		else
			printf("OPEN LE BOUG %d %s\n", main->cmd_info->outfile->fd, main->cmd_info->outfile->filename);
		if (dup2(main->cmd_info->outfile->fd, STDOUT_FILENO) == -1)
			perror("dup2 failed");
		if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
			close(main->cmd_info->outfile->fd);
	}
	if (execve(cmd_path, (char *const *)main->cmd_info->argv, envp) == -1)
	{
		if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
			close(main->cmd_info->outfile->fd);
		auto int exit_code = free_execve(&main);
		perror("execve failed");
		exit(exit_code);
	}
}

void	last_executor(t_main *main, char **envp, int tube, int onlyonecommand)
{
	pid_t	pid;

	if (pipe(main->cmd_info->tube) == -1)
	{
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
	close(main->cmd_info->tube[1]);
	// if (onlyonecommand == 1)
	// {
	// 	close(main->cmd_info->tube[0]);
	// 	main->cmd_info->tube[0] = -1;
	// }
	(void)onlyonecommand;
	pid = fork();
	if (pid == 0)
		last_child_executor(tube, main, main->cmd_info->cmd_path, envp);
	else
	{
		// close(main->cmd_info->tube[1]);
		if (tube != -1)
			close(tube);
		if (main->cmd_info->outfile != NULL && main->cmd_info->outfile->fd != -1)
		{
			close(main->cmd_info->outfile->fd);
			main->cmd_info->outfile->fd = -1;
		}
		add_pid(main, pid);
	}
	main->tube->fd = main->cmd_info->tube[0];
}

int	executor_setup(t_main **main, int *nbcmds, char *cmd)
{
	*nbcmds = totalcmds(cmd);
	(*main)->pids[0] = 0;
	setup_tube(*main);
	return (1);
}

int	onecmdexector(t_main *main, char **envp)
{
	if (main->cmd_info->cmd == NULL)
		return (handle_heredoc(main), create_out(main), end_pids(&main),
			free_all_cmd_info(&main), no_leaks(main), -1);
	setup_cmd_redirs(main->cmd_info);
	if (hasinfile(&main, 0) == -1)
	{
		return (free_cmd_info(&main->cmd_info),
			no_leaks(main), 0); // end_pids(&main, pids) retiré
	}
	if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
	{
		close(main->cmd_info->outfile->fd);
		main->cmd_info->outfile->fd = -1;
	}
	if (builtin_exec(main, &main->envp, 1, 1) == 1)
		return (1);
	if (main->cmd_info->infile != NULL)
		lcmd_searcher(main, envp, main->cmd_info->infile->fd, 1);
	else
		lcmd_searcher(main, envp, -1, 1);
	return (1);
}

int	multiplecmdexector(t_main *main, char **envp, int nbcmds, int onlyonecommand)
{
	if (tube_handler(&main) == -1)
		return (-1);
	// if (main->cmd_info->infile && fcntl(main->cmd_info->infile->fd, F_GETFD) != -1)
	// 	printf("Infile detected: %s\n", main->cmd_info->infile->filename);
	// else
	// 	printf("FD is not open or no redirection.\n");
	if (builtin_exec(main, &main->envp, nbcmds, 0) == 1)
		return (1);
	if (main->cmd_info->infile != NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
	{
		main->tube->fd = cmd_searcher(main, envp,
				main->cmd_info->infile->fd, onlyonecommand);
	}
	else if (main->cmd_info->infile == NULL && main->cmd_info->outfile == NULL
		&& nbcmds > 1)
	{
		printf("Mon noir pref\n");
		main->tube->fd = cmd_searcher(main, envp,
				main->tube->fd, onlyonecommand);
	}
	else if (main->cmd_info->infile == NULL)
	{
		lcmd_searcher(main, envp, main->tube->fd, onlyonecommand);
		printf("NON ALLO\n");	
		printf("\n\n\nEZx : %d %d\n\n\n", main->tube->fd, main->cmd_info->tube[0]);
		// main->tube->fd = -1;
	}
	else if (main->cmd_info->infile != NULL)
	{
		printf("NON INFILE\n");
		if (main->tube && main->tube->fd != -1)
		{
			close(main->tube->fd);
			main->tube->fd = -1;
		}
		lcmd_searcher(main, envp, main->cmd_info->infile->fd, onlyonecommand);
		// main->tube->fd = -1;
	}
	return (1);
}
