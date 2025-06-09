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
int		cd(int total_args, const char *path);
int		echo(int argc, const char **argv);
void	export(int argc, char **argv, t_envp **datas);
void	unset(int argc, char **argv, t_envp **datas);
void	env(t_envp *datas);
int		check_cmds(char *user_input, t_envp **datas, t_history *history);

#endif
