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

int	check_cmds(char *user_input, t_envp **datas, t_history *history)
{
	char	**args;
	int		total_args;

	args = ft_split(user_input, ' ');
	total_args = args_counter(args);
	if (ft_strncmp(user_input, "cd", 2) == 0)
		cd(total_args, args[1]);
	else if (ft_strncmp(user_input, "echo", 4) == 0)
		echo(total_args, (const char **)args);
	else if (ft_strncmp(user_input, "env", 3) == 0)
		env(*datas);
	else if (ft_strncmp(user_input, "export", 6) == 0)
		export(total_args, args, datas);
	else if (ft_strncmp(user_input, "unset", 5) == 0)
		unset(total_args, args, datas);
	else if (ft_strncmp(user_input, "history", 7) == 0)
		print_history(history);
	else if (ft_strncmp(user_input, "exit", 4) == 0)
		ft_exit(); 
	else
		return (0);
	return (1);
}
