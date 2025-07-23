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

char	*data_spliter(char	*str)
{
	int		i;
	int		j;
	char	*key;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	j = 0;
	key = malloc((i + 1) * sizeof(char));
	if (key == NULL)
		return (0);
	while (j < i && str[i])
	{
		key[j] = str[j];
		j++;
	}
	key[j] = '\0';
	return (key);
}

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
	main->tube = NULL;
	return (1);
}
