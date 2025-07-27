/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_main **main)
{
	t_cmd_info	*tmp_cmd_info;

	while ((*main)->cmd_info)
	{
		tmp_cmd_info = (*main)->cmd_info;
		(*main)->cmd_info = (*main)->cmd_info->next;
		if (tmp_cmd_info->infile && tmp_cmd_info->infile->fd > -1)
			close(tmp_cmd_info->infile->fd);
		if (tmp_cmd_info->outfile && tmp_cmd_info->outfile->fd > -1)
			close(tmp_cmd_info->outfile->fd);
		free_cmd_info(&tmp_cmd_info);
	}
	free_tokens(&(*main)->tokens);
	free_main(main);
	exit(EXIT_SUCCESS);
}
