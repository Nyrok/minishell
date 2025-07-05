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

int	args_counter(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

int	builtin_exec(t_cmd_info *cmd_info, t_envp **datas, t_history *history)
{
	if (ft_strncmp(cmd_info->cmd, "cd", 2) == 0)
		cd(cmd_info->argc, cmd_info->argv[1]);
	else if (ft_strncmp(cmd_info->cmd, "echo", 4) == 0)
		echo(cmd_info->argc, (const char **)cmd_info->argv);
	else if (ft_strncmp(cmd_info->cmd, "env", 3) == 0)
		env(*datas);
	else if (ft_strncmp(cmd_info->cmd, "export", 6) == 0)
		export(cmd_info->argc, cmd_info->argv, datas);
	else if (ft_strncmp(cmd_info->cmd, "unset", 5) == 0)
		unset(cmd_info->argc, cmd_info->argv, datas);
	else if (ft_strncmp(cmd_info->cmd, "history", 7) == 0)
		print_history(history);
	else if (ft_strncmp(cmd_info->cmd, "exit", 4) == 0)
		ft_exit();
	else
		return (0);
	return (1);
}