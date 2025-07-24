/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by cduquair          #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	changepwd(t_main *main)
{
	t_main	*actual;
	char	*buffer;

	actual = main;
	buffer = malloc(1024 * sizeof(char));
	actual->cmd_info->argv = malloc(2 * sizeof(char*));
	if (getcwd(buffer, 1024) == NULL)
		return (-1);
	actual->cmd_info->argv[1] = ft_strdup("PWD");
	unset(actual, 1, &main->envp);
	actual->cmd_info->argv[1] = ft_strjoin("PWD=", buffer);
	export(actual, 2, actual->cmd_info->argv, 1);
	return (1);
}

int	cd(t_main *main, int total_args, const char *path)
{
	if (total_args > 2)
	{
		printf("-minishell: Too many arguments");
		return (0);
	}
	if (!path)
	{
		printf("-minishell: Empty path provided.\n");
		return (0);
	}
	if (chdir(path) != 0)
	{
		printf("-minishell: Cannot enter into the folder.\n");
		return (0);
	}
	changepwd(main);
	if (main->tube != NULL && main->tube->fd >= 0)
		close(main->tube->fd);
	main->tube = NULL;
	return (1);
}
