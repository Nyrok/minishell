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

int	fd_opener(t_main **main, t_redir *actual_redir, int error_check, int print)
{
	(void)error_check;
	append_opener(actual_redir);
	if (actual_redir->io == STDIN_FILENO \
		&& access(actual_redir->filename, F_OK) != 0 \
			&& actual_redir->type != HEREDOC)
		return (no_such_file(actual_redir, print), -1); // -2 -> -1
	else if (access(actual_redir->filename, R_OK) != 0 && \
			actual_redir->type != HEREDOC)
		return (permission_denied(main, actual_redir, print), -1); // -2 -> -1
	if (actual_redir->type == APPEND)
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (actual_redir->type == REDIN)
		actual_redir->fd = open(actual_redir->filename, O_RDONLY, 0444);
	//else if (actual_redir->type == HEREDOC && error_check == 0 && print == 1)
	//{
	//	printf("TTT\n"); // le print == 1 permet de lancer un heredoc une seule fois
	//					// comme 2eme idee de correction on peut appeler fd opener dans chaque exec au lieu de hasinfile
	//	actual_redir->fd = ft_heredoc(actual_redir->filename);
	//}
	else if (actual_redir->type == REDOUT && actual_redir->good == 1)
		actual_redir->fd = open(actual_redir->filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	else if (actual_redir->type == REDOUT && actual_redir->good == 0)
		actual_redir->fd = open(actual_redir->filename, O_CREAT, 0777);
	if (actual_redir->fd == -1 && error_check == 0) // Ce cas arrive si il n'a rien uvert ou pas reussi a ouvrir le soucis c que bah ca nique si y'a un heredoc
	{
		//printf("SASASAS %d\n", actual_redir->type); // apparait dans heredoc avec une cmd + ctrl c
		return (-1);
	}
	if (actual_redir->good == 0 && actual_redir->fd != -1) // ajout du check si f != -1 car sinon si je fais << eof << eof2 bah eof2 n'est pas good pourtant il est deja ferme donc invalid close
	{
		close(actual_redir->fd);
	}
	return (1);
}

void	check_tube(t_main **main)
{
	if ((*main)->tube == NULL)
	{
		(*main)->pids = ft_calloc((count_cmd_info((*main)->cmd_info) + 1), \
			sizeof(pid_t));
		setup_tube(*main);
	}
}

int	create_out(t_main *main)
{
	t_redir	*redir_tmp;

	while (main->cmd_info->redirs)
	{
		redir_tmp = main->cmd_info->redirs;
		main->cmd_info->redirs = main->cmd_info->redirs->next;
		if (redir_tmp->type == REDOUT)
		{
			// redir_tmp->fd = open(redir_tmp->filename,
			// 		O_CREAT | O_WRONLY | O_TRUNC, 0777);
			redir_tmp->fd = open(redir_tmp->filename,
					O_CREAT, 0777); // create only car je crois pas qu'il faille supp les infos mais a check
			if (redir_tmp->fd == -1)
				return (-1);
			close(redir_tmp->fd);
		}
		free(redir_tmp->filename);
		free(redir_tmp);
	}
	return (1);
}

int	handle_heredoc(t_main *main)
{
	t_redir	*redir_tmp;
	int		fd;

	redir_tmp = main->cmd_info->redirs;
	while (redir_tmp)
	{
		if (redir_tmp->type == HEREDOC)
		{
			fd = ft_heredoc(redir_tmp->filename);
			if (fd != -1)
			{
				close(fd);
				return (-1);
			}
		}
		redir_tmp = redir_tmp->next;
	}
	return (-1);
}

int	handle_multiple_cmds(t_main *main, char **envp, int nbcmds, int *error_printed)
{
	//auto int has_infile = 0;
	setup_cmd_redirs(main->cmd_info);
	check_tube(&main);
	(void)error_printed;
	//has_infile = hasinfile(&main, 0, error_printed);
	//printf("a = %d\n", has_infile);
	//if (has_infile == -2 && main->tube)
	//{
	//	main->tube->fd = create_eof_fd(main);
	//}
	//else if (has_infile != -1)
	//{
	fdcls(&main, 0);
	multiplecmdexector(main, envp, nbcmds);
	//}
	//else if (has_infile == -1)
	//{
	//	return (-1);
	//}
	fdcls(&main, 0);
	return (0);
}

int hasinfile_heredocs_only(t_main *main)
{
	t_cmd_info *cmd = main->cmd_info;
	while (cmd)
	{
		t_redir *redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == HEREDOC)
            {
                redir->fd = ft_heredoc(redir->filename);
                if (redir->fd == -1)
                    return (-1);
				//if (main->cmd_info->cmd == NULL && redir->fd != -1) // Jai retire ca pour test car << eof << eof2 avait des invalid close
				//{
				//	close(redir->fd);
				//	redir->fd = -1;
				//}
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return (0);
}

int	multiple_cmd_handler(t_main *main, char **envp, int nbcmds)
{
	t_cmd_info	*temp_cmd_info;

	auto int error_printed = 1;
	auto int ctrlc_heredoc = 0;
	if (nbcmds == 0)
		free_cmd_info(&main->cmd_info);
	ctrlc_heredoc = hasinfile_heredocs_only(main);
	while (nbcmds > 0)
	{
		if (ctrlc_heredoc == -1) // Gestion de si on ctrl c un heredoc pour ne pas lire la suite des cmds
		{
			free_all_cmd_info(&main);
			break ; 
		}
		if (main->cmd_info && main->cmd_info->cmd != NULL)
			handle_multiple_cmds(main, envp, nbcmds, &error_printed);
		else
		{
			if (main->cmd_info)
			{
				main->last_exit_status = 1;
				fork_bad_file(main);
				hasinfile2(&main, 0, 1); // on gère par exemple < in | cat ou < invalid | cat ici
				fdcls(&main, 0);
				// create_out(main);
				// main->tube->fd = create_eof_fd(main);
			}
		}
		nbcmds--;
		temp_cmd_info = main->cmd_info;
		main->cmd_info = main->cmd_info->next;
		free_cmd_info(&temp_cmd_info);
	}
	main->cmd_info = NULL;
	delete_tube(main);
	return (1);
}
