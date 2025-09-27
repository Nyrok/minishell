/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_history	*list_history_init(char *cmd)
{
	t_history	*list_history;

	list_history = malloc(sizeof(t_history));
	if (!list_history)
		return (NULL);
	list_history->cmd = ft_strdup(cmd);
	list_history->next = NULL;
	return (list_history);
}

void	list_history_add(t_history **history, char *cmd)
{
	t_history	*actual;

	actual = *history;
	while (actual->next != NULL)
		actual = actual->next;
	actual->next = malloc(sizeof(t_history));
	if (!actual->next)
		return ;
	actual->next->cmd = ft_strdup(cmd);
	actual->next->next = NULL;
}

void	list_history_cleaner(t_main *main)
{
	t_history	*actual;
	t_history	*tmp;

	actual = main->history;
	while (actual != NULL)
	{
		tmp = actual;
		actual = actual->next;
		free(tmp->cmd);
		free(tmp);
	}
	main->history = NULL;
}

void	print_history(t_history *history)
{
	t_history	*actual;
	int			i;

	actual = history;
	i = 1;
	while (actual != NULL)
	{
		printf("    %d %s\n", i, actual->cmd);
		actual = actual->next;
		i++;
	}
}
