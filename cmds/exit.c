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

int	ft_isgood(t_main *main)
{
	auto int i = 0;
	if (main->cmd_info->argv[i][0] == '+')
		i = 1;
	else if (main->cmd_info->argv[i][0] == '-')
	{
		main->last_exit_status = 244;
		return (0);
	}
	while (main->cmd_info->argv[1][i])
	{
		if (ft_isdigit(main->cmd_info->argv[1][i]) == 0 \
			|| ft_strlen(main->cmd_info->argv[1]) > 11)
		{
			main->last_exit_status = 2;
			printf("minishell: exit: %s: numeric argument required\n",
				main->cmd_info->argv[i]);
			return (0);
		}
		i++;
	}
	main->last_exit_status = 0;
	return (1);
}

void	clear_tube(t_main **main)
{
	if ((*main)->pids != NULL && *(*main)->pids != 0)
	{
		end_pids(main);
		(*main)->pids = NULL;
	}
	if ((*main)->tube->fd != -1)
		close((*main)->tube->fd);
	free((*main)->tube);
	(*main)->tube = NULL;
	if ((*main)->cmd_info->argc == 2 && ft_isgood(*main))
		(*main)->last_exit_status = ft_atoi((*main)->cmd_info->argv[1]);
}

void	free_exit(t_main **main)
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
	if ((*main)->pids)
		free((*main)->pids);
	printf("exit\n");
}

int	exit_checker(t_main **main, int nbcmds, int onlyonecmd)
{
	if (nbcmds == 1 && (*main)->tube->fd == -1 && (*main)->cmd_info->infile && onlyonecmd == 1)
		return (0);
	if ((*main)->tube->fd != -1 || (*main)->cmd_info->infile || nbcmds > 1 || onlyonecmd == 0) // Pk quand y'a un infile ???
	{
		return (1);
	}
	return (0);
}

void	ft_exit(t_main **main, int nbcmds, int onlyonecmd)
{
	auto int exit_code = 0;
	if ((*main)->cmd_info->argc > 2)
	{
		printf("minishell: exit: too many arguments\n");
		if ((*main)->tube->fd != -1)
			clear_tube(main);
		return ;
	}
	if ((*main)->cmd_info->argc == 2)
		ft_isgood(*main);
	if (exit_checker(main, nbcmds, onlyonecmd) == 1) // Pk quand y'a un infile ???
		clear_tube(main);
	else
	{
		if ((*main)->cmd_info->argc == 2)
			exit_code = ft_atoi((*main)->cmd_info->argv[1]);
		clear_tube(main);
		if ((*main)->str_envp)
			free((*main)->str_envp);
		(*main)->str_envp = NULL;
		free_exit(main);
		free_tokens(&(*main)->tokens);
		free_main(main);
		exit(exit_code);
	}
}
