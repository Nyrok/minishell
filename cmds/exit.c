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

//void	ft_exit(t_main **main, pid_t *pids)
//{
//	t_cmd_info	*tmp_cmd_info;

//	free(pids);
//	while ((*main)->cmd_info)
//	{
//		tmp_cmd_info = (*main)->cmd_info;
//		(*main)->cmd_info = (*main)->cmd_info->next;
//		if (tmp_cmd_info->infile && tmp_cmd_info->infile->fd > -1)
//			close(tmp_cmd_info->infile->fd);
//		if (tmp_cmd_info->outfile && tmp_cmd_info->outfile->fd > -1)
//			close(tmp_cmd_info->outfile->fd);
//		free_cmd_info(&tmp_cmd_info);
//		if ((*main)->tube)
//		{
//			if ((*main)->tube->fd != -1)
//				close((*main)->tube->fd);
//			free((*main)->tube->filename);
//			free((*main)->tube);
//		}
//	}
//	free_tokens(&(*main)->tokens);
//	free_main(main);
//	exit(EXIT_SUCCESS);
//}

void	clear_tube(t_main **main, pid_t **pids)
{
	printf("MAIS NAN \n");
	end_pids(pids);
	*pids = NULL;
	if ((*main)->tube->fd != -1)
		close((*main)->tube->fd);
	// free((*main)->tube->filename);
	free((*main)->tube);
	(*main)->tube = NULL;
}

void	ft_exit(t_main **main, pid_t **pids)
{
	t_cmd_info	*tmp_cmd_info;

	if ((*main)->tube->fd != -1)
		clear_tube(main, pids);
	else
	{
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
		clear_tube(main, pids);
		free_tokens(&(*main)->tokens);
		free_main(main);
		exit(EXIT_SUCCESS);
	}
}
