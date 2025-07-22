/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

# include "minishell.h"

typedef struct s_cmd_info	t_cmd_info;

typedef struct s_envp
{
	char			*data;
	struct s_envp	*next;
}	t_envp;

typedef struct s_history
{
	char				*cmd;
	struct s_history	*next;
}	t_history;

t_envp	*add_cell(char *str);
t_envp	*list_maker(char **envp);
char	*data_spliter(char *str);
// int		cd(t_main *main, int total_args, const char *path, int is_last);
// int		echo(t_main *main, int argc, const char **argv, int is_last);
// void		export(t_main *main, int argc,
//				char **argv, t_envp **datas, int is_last);
// void	unset(t_main *main, int argc, char **argv, t_envp **datas, int is_last);
// void	env(t_main *main, t_envp *datas, int is_last);
// int		pwd(t_main *main, int is_last)
// void	ft_exit(void);
// int		builtin_exec(t_main *main, t_cmd_info *cmd_info, t_envp **datas,
//				t_history *history, int nbcmds);

#endif
