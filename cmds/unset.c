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


#include "../minishell.h"

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
	while (j < i && str[i])
	{
		key[j] = str[j];
		j++;
	}
	key[j] = '\0';
	return (key);
}

void	remove_first(t_export **datas, t_export *actual, t_export *temp)
{
	temp = actual->next;
	*datas = temp;
}

void	unset_list_maker(int argc, char **argv, t_export **datas)
{
	t_export	*actual;
	t_export	*temp;
	t_export	*previous;
	char		*key;
	int			i;

	previous = NULL;
	i = 0;
	while (i < argc - 1)
	{
		actual = *datas;
		while (actual != NULL)
		{
			key = data_spliter(actual->data);
			if (previous == NULL && ft_strcmp(key, argv[i + 1]) == 0)
				remove_first(datas, actual, temp);
			else if (actual->next != NULL && ft_strcmp(key, argv[i + 1]) == 0)
			{
				temp = actual->next;
				actual = previous;
				actual->next = temp;
			}
			else if (ft_strcmp(key, argv[i + 1]) == 0)
				previous->next = NULL;
			previous = actual;
			actual = actual->next;
			free(key);
		}
		i++;
	}
}

void	unset(int argc, char **argv, t_export **datas)
{
	if (datas == NULL || argc == 0)
		return ;
	unset_list_maker(argc, argv, datas);
}
