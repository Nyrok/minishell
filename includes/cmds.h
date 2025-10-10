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

int		cd(t_main *main, int total_args, const char *path, int onlyonecommand);
int		echo(t_main *main, int argc, const char **argv, int nbcmds);
int		export(t_main *main, int argc, char **argv, int nbcmds);
int		unset(t_main *main, int argc, t_envp **datas);
int		env(t_main *main, t_envp *envp, int onlyonecommand, int nbcmds);
int		pwd(t_main *main, int nbcmds);
void	ft_exit(t_main **main, int nbcmds, int onlyonecmd);
int		builtin_exec(t_main *main, t_envp **envp, int nbcmds, int onlyonecmd);
void	clear_tube(t_main **main);
int		check_outfile(t_main *main, int *fd, int nbcmds);
void	print_env(int fd, t_envp *actual, int prefix_export);

#endif
