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

#include "cmds.h"

void	check_cmds(char *user_input)
{

	if (ft_strncmp(user_input, "cd", 2) == 0)
		printf("cd\n");
	else if (ft_strncmp(user_input, "echo", 4) == 0)
		printf("echo\n");
	else if (ft_strncmp(user_input, "env", 3) == 0)
		printf("env\n");
	else if (ft_strncmp(user_input, "export", 6) == 0)
		printf("export\n");
	else if (ft_strncmp(user_input, "unset", 5) == 0)
		printf("unset\n");
}
