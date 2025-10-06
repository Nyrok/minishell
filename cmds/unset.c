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

static void	*free_envp_single(t_envp **envp)
{
	free((*envp)->key);
	free((*envp)->value);
	free((*envp)->full);
	free(*envp);
	*envp = NULL;
	return (NULL);
}

static void	rm_first(t_envp **envp, t_envp *actual, t_envp **to_free)
{
	*to_free = actual;
	*envp = actual->next;
}

static void	*unset_loop(t_main *main, t_envp *actual, t_envp *previous, int i)
{
	auto t_envp * temp = NULL;
	auto t_envp * to_free = NULL;
	while (actual != NULL)
	{
		if (!previous && !ft_strcmp(actual->key, main->cmd_info->argv[i + 1]))
			rm_first(&main->envp, actual, &to_free);
		else if (actual->next != NULL && ft_strcmp(actual->key,
				main->cmd_info->argv[i + 1]) == 0)
		{
			temp = actual->next;
			to_free = actual;
			actual = previous;
			actual->next = temp;
		}
		else if (ft_strcmp(actual->key, main->cmd_info->argv[i + 1]) == 0)
		{
			previous->next = NULL;
			to_free = actual;
		}
		if (to_free)
			return (free_envp_single(&to_free));
		previous = actual;
		actual = actual->next;
	}
	return (NULL);
}

static void	unset_list_maker(t_main *main, t_envp **envp)
{
	t_envp	*actual;
	t_envp	*previous;

	previous = NULL;
	auto int i = 0;
	while (i < main->cmd_info->argc - 1)
	{
		actual = *envp;
		unset_loop(main, actual, previous, i);
		i++;
	}
}

int	unset(t_main *main, int argc, t_envp **envp)
{
	if (envp == NULL || argc == 0)
		return (-1);
	unset_list_maker(main, envp);
	if (main->tube != NULL && main->tube->fd >= 0)
	{
		close(main->tube->fd);
		main->tube->fd = -1;
	}
	main->last_exit_status = 0;
	return (1);
}
