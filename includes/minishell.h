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

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h> 
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"
# include "cmds.h"
# include "executor.h"
# include "tokenizer.h"
# include "parser.h"
# include "free.h"

# define PURPLE  "\033[0;35m"
# define RESET   "\033[0m"

struct s_main
{
	t_envp			*datas;
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
void		history_cleaner(t_history **history);
void		print_history(t_history *history);
char		**envp_to_str(t_envp *envp);
int			executor(char *cmd, struct s_main *main);
int			builtin_exec(t_main *main, t_cmd_info *cmd_info, t_envp **datas, t_history *history, int nbcmds);
int			cd(t_main *main, int total_args, const char *path);
int			echo(t_main *main, int argc, const char **argv, int nbcmds);
void		export(t_main *main, int argc, char **argv, t_envp **datas, int nbcmds);
void		unset(t_main *main, int argc, char **argv, t_envp **datas);
int			env(t_main *main, t_envp *datas, int nbcmds);
int			pwd(t_main *main, int nbcmds);
void		ft_exit(void);
int			builtin_exec(t_main *main, t_cmd_info *cmd_info, t_envp **datas, t_history *history, int nbcmds);

#endif
