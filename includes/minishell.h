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
	char			**str_envp;
	t_cmds_paths	*cmds_paths;
	t_history		*history;
	t_token			*tokens;
	t_cmd_info		*cmd_info;
	int				last_exit_status;
	t_redir			*tube;
	pid_t			*pids;
};

void		line_reader(t_main *main_struct);

t_history	*list_history_init(char *cmd);
void		list_history_add(t_history **history, char *cmd);
void		list_history_cleaner(t_main *main);
void		print_history(t_history *history);
int			executor(char *cmd, struct s_main *main);
//int			file_executor(t_main *main, int file, int last);
void		last_executor(t_main *main, char **envp, int tube,
				int i);
int			cmd_executor(t_main *main, char **envp, int file, int i);
int			totalcmds(char *cmd);
int			multiplecmdexector(t_main *main,
				char **envp, int nbcmd);
int			onecmdexector(t_main *main, char **envp);
void		add_pid(t_main *main, pid_t newpid);
void		end_pids(t_main **main);
int			no_leaks(t_main *main);
int			executor_setup(t_main **main, int *nbcmds, char *cmd);
int			print_error(t_main *main, int error_code, int cmd_found);
int			cmd_searcher(t_main *main, char **envp, int file);
void		lcmd_searcher(t_main *main, char **envp, int tube);
int			hasinfile(struct s_main **main, int error_check, int *print);
void		setup_tube(t_main *main);
void		reset_tube(t_main *main);
int			fd_opener(t_main **main, t_redir *actual_redir,
				int error_check, int print);
int			multiple_cmd_handler(t_main *main,
				char **envp, int nbcmds);
void		free_redir(t_redir **redir);
int			create_out(t_main *main);
int			handle_heredoc(t_main *main);
void		handle_signal(int signal);
int			check_access(t_main *main, int j, char *filename);
int			isfilevalid(t_main *main);
int			tube_handler(t_main **main);
int			fdcls(t_main **main, int error);
int			ft_heredoc(char *end);
int			free_execve(t_main **main);
void		free_envp(t_envp **envp);
int			no_such_file(t_redir *actual_redir, int print);
int			permission_denied(t_main **main, t_redir *actual_redir, int print);
void		delete_tube(t_main *main);
void		append_opener(t_redir *actual_redir);
void		ft_dup2(int src, int dest);
int			isbuilt_in(t_main *main);
int			ft_execperm(t_main *main, char *pathname);
void		close_outfile(t_main *main);
char		*paths_searcher(char *cmd, char *cmd_path, char *paths);
int			create_eof_fd(t_main *main);
int			check_if_exist(t_main *main);
int			hasinfile_heredocs_only(t_main *main);
void		fork_bad_file(t_main *main);
int			hasinfile2(struct s_main **main, int error_check, int print);
void		relative_path_executor(t_main *main, char **envp, int lastcmd);
void		close_heredoc_future_cmds(t_main *main);
int			ft_exist(t_main *main, char *pathname);
int			ft_isadirectory(t_main *main, char *pathname);
int			ft_commandisapoint(t_main *main);
int			ft_commandonlypoints(t_main *main);
int			isonlypt(char *str);
void		free_cmd_path(t_main *main);
void		free_main_paths(t_main *main);
void		end_fd(int fd);
void		ft_strdup_cmd_path(t_main *main, int i);
int			isonlypt(char *str);
int			gestion_error_relative_path(t_main *main, int type);
void		relative_path_free(t_main *main);
int			error_no_path(t_main *main, int error);
void		relative_path_executor(t_main *main, char **envp, int lastcmd);
void		cmd_null(t_main *main);

#endif
