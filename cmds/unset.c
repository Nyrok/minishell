/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_first(t_envp **envp, t_envp *actual, t_envp *temp)
{
	temp = actual->next;
	*envp = temp;
}

void	unset_list_loop(t_main *main, t_envp *actual, t_envp *previous, int i)
{
	t_envp	*temp;

	while (actual != NULL)
	{
		if (previous == NULL && ft_strcmp(actual->key,
				main->cmd_info->argv[i + 1]) == 0)
			remove_first(&main->envp, actual, temp);
		else if (actual->next != NULL && ft_strcmp(actual->key,
				main->cmd_info->argv[i + 1]) == 0)
		{
			temp = actual->next;
			actual = previous;
			actual->next = temp;
		}
		else if (ft_strcmp(actual->key, main->cmd_info->argv[i + 1]) == 0)
			previous->next = NULL;
		previous = actual;
		actual = actual->next;		
		if (ft_strcmp(previous->key, main->cmd_info->argv[i + 1]) == 0)
		{
			free_envp(&previous);
			break ;
		}
	}
}

void	unset_list_maker(t_main *main, t_envp **envp)
{
	t_envp	*actual;
	t_envp	*previous;

	previous = NULL;
	auto int i = 0;
	while (i < main->cmd_info->argc - 1)
	{
		actual = *envp;
		unset_list_loop(main, actual, previous, i);
		i++;
	}
}

int	unset(t_main *main, int argc, t_envp **envp)
{
	if (envp == NULL || argc == 0)
		return (-1);
	unset_list_maker(main, envp);
	if (main->tube != NULL && main->tube->fd >= 0)
		close(main->tube->fd);
	return (1);
}
