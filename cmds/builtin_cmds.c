/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exec(t_main *main, t_envp **envp, int nbcmds, int onlyonecmd)
{
	delete_tube(main);
	if (ft_strcmp(main->cmd_info->cmd, "cd") == 0)
		cd(main, main->cmd_info->argc, main->cmd_info->argv[1]);
	else if (ft_strcmp(main->cmd_info->cmd, "echo") == 0)
		echo(main, main->cmd_info->argc, (const char **)main->cmd_info->argv,
			nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "env") == 0)
		env(main, *envp, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "export") == 0)
		export(main, main->cmd_info->argc, main->cmd_info->argv, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "unset") == 0)
		unset(main, main->cmd_info->argc, envp);
	else if (ft_strcmp(main->cmd_info->cmd, "pwd") == 0)
		pwd(main, nbcmds);
	else if (ft_strcmp(main->cmd_info->cmd, "history") == 0 \
		&& main->cmd_info->argc >= 2 \
		&& ft_strncmp(main->cmd_info->argv[1], "-c", 2) == 0)
		list_history_cleaner(main);
	else if (ft_strcmp(main->cmd_info->cmd, "history") == 0)
		print_history(main->history);
	else if (ft_strcmp(main->cmd_info->cmd, "exit") == 0)
		ft_exit(&main, nbcmds, onlyonecmd);
	else
		return (0);
	return (1);
}
