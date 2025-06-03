/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmds.h"

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

t_export	*list_maker(int argc, char **argv, char **envp)
{
	t_export	*datas;
	t_export	*actual;
	char		*key;
	int			i;

	i = 0;
	datas = add_cell(envp[i]);
	actual = datas;
	key = data_spliter(envp[i]);
	while (envp[i] != NULL)
	{
		if (ft_strcmp(key, argv[1]) != 0)
		{
			actual->next = add_cell(envp[i]);
			actual = actual->next;
		}
		i++;
	}
	free(key);
	return (datas);
}

int	unset(int argc, char **argv, char **envp)
{
	t_export	*datas;
	t_export	*actual;
	int			i;

	if (envp[0] == NULL || args_checker(argc) == 0)
		return (0);
	datas = list_maker(argc, argv, envp);
	actual = datas;
	while (actual != NULL)
	{
		printf("%s\n", actual->data);
		actual = actual->next;
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	unset(argc, argv, envp);
}
