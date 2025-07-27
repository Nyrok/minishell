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
# include "fwd.h"

typedef struct s_history
{
	char				*cmd;
	struct s_history	*next;
}	t_history;

int		cd(t_main *main, int total_args, const char *path);
int		echo(t_main *main, int argc, const char **argv, int nbcmds);
int		export(t_main *main, int argc, char **argv, int nbcmds);
int		unset(t_main *main, int argc, t_envp **datas);
int		env(t_main *main, t_envp *datas, int nbcmds);
int		pwd(t_main *main, int nbcmds);
void	ft_exit(t_main **main, pid_t **pids);
int		builtin_exec(t_main *main, pid_t **pids, t_envp **envp, int nbcmds);

#endif
