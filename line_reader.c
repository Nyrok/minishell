/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	line_reader(t_main	 *main_struct)
{
	char		*user_input;

	while (1)
	{
		user_input = readline(PURPLE "minishell>" RESET);
		if (!user_input)
			anti_leaks(&main_struct);
		if (check_cmds(user_input, &main_struct->datas) == 0)
			cmd_searcher(user_input, main_struct->cmds_paths->paths, main_struct->datas);
		free(user_input);
	}
}