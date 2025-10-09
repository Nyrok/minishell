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
	signal(SIGQUIT, SIG_DFL);
	close_heredoc_future_cmds(main);
	if (main->cmd_info->tube[0] != -1 && tube != main->cmd_info->tube[0])
		close(main->cmd_info->tube[0]);
	if (tube != -1)
	{
		ft_dup2(tube, STDIN_FILENO);
		close(tube);
		tube = -1;
	}
	if (main->tube && main->tube->fd != -1)
		close(main->tube->fd);
	if (main->cmd_info->outfile != NULL)
	{
		fd_opener(&main, main->cmd_info->outfile, 0, 1); // 0 -> -1 useless je pense
		ft_dup2(main->cmd_info->outfile->fd, STDOUT_FILENO);
		close_outfile(main);
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

void	last_executor(t_main *main, char **envp, int tube, int i)
{
	pid_t	pid;

	if (i == -2 || (i != -1 && main->cmds_paths->paths[i] == NULL)) // pas sûr de la condition // edit pas sur du tt car on check pas if exist et on ne met pas le tube a null je crois
	{
		//printf("CC\n"); 
		print_error(main, NOTFOUND, 0);
		fork_bad_file(main);
		if (tube != -1)
			close(tube);
		if (main->cmd_info->outfile != NULL
			&& main->cmd_info->outfile->fd != -1)
		{
			close(main->cmd_info->outfile->fd);
			main->cmd_info->outfile->fd = -1;
		}
		return ;
	}
	if (pipe(main->cmd_info->tube) == -1)
	{
		perror("pipe failed");
		return ;
	}
	close(main->cmd_info->tube[1]);
	pid = fork();
	if (pid == 0)
		last_child_executor(tube, main, main->cmd_info->cmd_path, envp);
	else
	{
		if (tube != -1)
			close(tube);
		if (main->cmd_info->outfile != NULL
			&& main->cmd_info->outfile->fd != -1)
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
	(void)cmd;
	*nbcmds = count_cmd_info((*main)->cmd_info);
	(*main)->pids[0] = 0;
	setup_tube(*main);
	return (1);
}

int create_eof_fd(t_main *main)
{
    int pipefd[2];
    
	if (main->tube && main->tube->fd != -1)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    close(pipefd[1]);
    return (pipefd[0]);
}

int	onecmdexector(t_main *main, char **envp)
{
	auto int error_printed = 1;
	auto int has_infile = 0;
	if (main->cmd_info && main->cmd_info->cmd == NULL)
		return (hasinfile_heredocs_only(main), main->last_exit_status = 1, fork_bad_file(main),
		hasinfile2(&main, 0, 1), fdcls(&main, 0), end_pids(&main),
		free_all_cmd_info(&main), no_leaks(main), -1);
	setup_cmd_redirs(main->cmd_info);
	hasinfile_heredocs_only(main);
	has_infile = hasinfile(&main, 0, &error_printed);
	if (has_infile == -1 || has_infile == -2)
		return (main->last_exit_status = 1, fork_bad_file(main), free_cmd_info(&main->cmd_info), no_leaks(main), 0);
	if (main->cmd_info->outfile && main->cmd_info->outfile->fd != -1)
	{
		close(main->cmd_info->outfile->fd);
		main->cmd_info->outfile->fd = -1;
	}
	if (builtin_exec(main, &main->envp, 1, 1) == 1)
		return (1);
	if (main->cmd_info->infile != NULL)
		lcmd_searcher(main, envp, main->cmd_info->infile->fd);
	else
	{
		lcmd_searcher(main, envp, -1);
	}
	if (main->tube && main->tube->fd != -1)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
	return (1);
}

int	hasinfile2(struct s_main **main, int error_check, int print)
{
	t_redir		*actual_redir;
	int			total;
	int			has_infile;

	total = 0;
	actual_redir = (*main)->cmd_info->redirs;
	(void)error_check;
	while (actual_redir != NULL)
	{
		has_infile = fd_opener(main, actual_redir, -1, print);
		if (has_infile == -1)
			return (-1);
		if (has_infile == -2)
			return (-2);
		if (actual_redir->type == 3)
			total = 1;
		actual_redir = actual_redir->next;
	}
	return (total);
}

void	fork_bad_file(t_main *main)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid == 0)
	{
		exit_code = main->last_exit_status;
		printf("Exit status : %d\n", exit_code);
		free_execve(&main);
		exit(exit_code);
	}
	else
		add_pid(main, pid);
	main->tube->fd = create_eof_fd(main);
}

int	multiplecmdexector(t_main *main, char **envp, int nbcmd)
{
	if (hasinfile2(&main, 0, 1) == -1)
	{
		fork_bad_file(main);
		return (-1);
	}
	fdcls(&main, 0);
	if (tube_handler(&main) == -1)
		return (-1);
	if (builtin_exec(main, &main->envp, nbcmd, 0) == 1)
		return (1);
	if (main->cmd_info->infile != NULL && main->cmd_info->outfile == NULL
		&& nbcmd > 1)
	{
		main->tube->fd = cmd_searcher(main, envp, main->cmd_info->infile->fd);
	}
	else if (main->cmd_info->infile == NULL && main->cmd_info->outfile == NULL
		&& nbcmd > 1)
	{
		main->tube->fd = cmd_searcher(main, envp, main->tube->fd);
	}
	else if (main->cmd_info->infile == NULL)
		lcmd_searcher(main, envp, main->tube->fd);
	else if (main->cmd_info->infile != NULL)
	{
		delete_tube(main);
		lcmd_searcher(main, envp, main->cmd_info->infile->fd);
	}
	return (1);
}
