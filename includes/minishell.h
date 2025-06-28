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

# define PURPLE  "\033[0;35m"
# define RESET   "\033[0m"

typedef struct s_main
{
	t_envp			*datas;
	t_cmds_paths	*cmds_paths;
	t_history		*history;
	t_token			*tokens;
}	t_main;

void		line_reader(t_main *main_struct);
void		anti_leaks(t_main **main_struct);
t_history	*list_history_init(char *cmd);
void		list_history_add(t_history **history, char *cmd);
void		history_cleaner(t_history **history);
void		print_history(t_history *history);
char		**envp_to_str(t_envp *envp);

#endif
