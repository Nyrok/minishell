/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isbuilt_in(t_main *main)
{
	if (ft_strcmp(main->cmd_info->cmd, "cd") == 0
		|| ft_strcmp(main->cmd_info->cmd, "echo") == 0
		|| ft_strcmp(main->cmd_info->cmd, "env") == 0
		|| ft_strcmp(main->cmd_info->cmd, "export") == 0
		|| ft_strcmp(main->cmd_info->cmd, "unset") == 0
		|| ft_strcmp(main->cmd_info->cmd, "pwd") == 0
		|| ft_strcmp(main->cmd_info->cmd, "history") == 0
		|| ft_strcmp(main->cmd_info->cmd, "history") == 0
		|| ft_strcmp(main->cmd_info->cmd, "exit") == 0)
	{
		return (1);
	}
	return (0);
}
