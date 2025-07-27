/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h> 
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include "libft.h"
# include "cmds.h"
# include "executor.h"
# include "tokenizer.h"
# include "parser.h"
# include "free.h"
# include "env.h"
# include "fwd.h"

# define PURPLE  "\033[0;35m"
# define RESET   "\033[0m"

struct s_main
{
	t_envp			*envp;
	t_cmds_paths	*cmds_paths;
	t_history		*history;
	t_token			*tokens;
	t_cmd_info		*cmd_info;
	t_redir			*tube;
};

void		line_reader(t_main *main_struct);
void		anti_leaks(t_main **main_struct);
t_history	*list_history_init(char *cmd);
void		list_history_add(t_history **history, char *cmd);
void		list_history_cleaner(t_main *main);
void		print_history(t_history *history);
int			executor(char *cmd, struct s_main *main);
int			file_executor(t_main *main, int file, pid_t **pids, int last);
void		last_executor(t_main *main, char **envp, int tube, pid_t **pids);
int			cmd_executor(t_main *main, char **envp, int file, pid_t **pids);
int			totalcmds(char *cmd);
int			multiplecmdexector(t_main *main,
				char **envp, pid_t *pids, int nbcmds);
int			onecmdexector(t_main *main, char **envp, pid_t *pids);
void		add_pid(pid_t **pids, pid_t newpid);
void		end_pids(pid_t **pids);
int			no_leaks(t_main *main, char **envp);
int			executor_setup(t_main *main, pid_t *pids, int *nbcmds, char *cmd);
int			print_error(t_main *main, int error_code, int cmd_found);
int			cmd_searcher(t_main *main, char **envp, int file, pid_t **pids);
void		lcmd_searcher(t_main *main, char **envp, int tube, pid_t **pids);
int			hasinfile(struct s_main *main);
void		setup_tube(t_main *main);
void		reset_tube(t_main *main);
int			fd_opener(t_redir *actual_redir);
void		multiple_cmd_handler(t_main *main,
				char **envp, pid_t *pids, int nbcmds);

#endif
