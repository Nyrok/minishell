/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exec(t_main *main, t_cmd_info *cmd_info, t_envp **datas, t_history *history, int nbcmds)
{
	if (ft_strncmp(cmd_info->cmd, "cd", 2) == 0)
		cd(main, cmd_info->argc, cmd_info->argv[1]);
	else if (ft_strncmp(cmd_info->cmd, "echo", 4) == 0)
		echo(main, cmd_info->argc, (const char **)cmd_info->argv, nbcmds);
	else if (ft_strncmp(cmd_info->cmd, "env", 3) == 0)
		env(main, *datas, nbcmds);
	else if (ft_strncmp(cmd_info->cmd, "export", 6) == 0)
		export(main, cmd_info->argc, cmd_info->argv, datas, nbcmds);
	else if (ft_strncmp(cmd_info->cmd, "unset", 5) == 0)
		unset(main, cmd_info->argc, cmd_info->argv, datas);
	else if (ft_strncmp(cmd_info->cmd, "pwd", 3) == 0)
		pwd(main, nbcmds);
	else if (ft_strncmp(cmd_info->cmd, "history", 7) == 0)
		print_history(history);
	else if (ft_strncmp(cmd_info->cmd, "exit", 4) == 0)
		ft_exit();
	else
		return (0);
	return (1);
}