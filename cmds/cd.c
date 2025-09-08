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
	actual->cmd_info->argv = malloc(3 * sizeof(char *));
	if (getcwd(buffer, 1024) == NULL)
		return (-1);
	actual->cmd_info->argv[0] = ft_strdup("cd");
	actual->cmd_info->argv[1] = ft_strdup("PWD");
	actual->cmd_info->argv[2] = NULL;
	unset(actual, 1, &main->envp);
	actual->cmd_info->argv[1] = ft_strjoin("PWD=", buffer);
	export(actual, 2, actual->cmd_info->argv, 1);
	return (1);
}

int	cd_home(t_main *main)
{
	char	*home;

	home = get_env_value(main->envp, "HOME");
	if (!home)
	{
		printf("-minishell: HOME not set\n");
		main->last_exit_status = 1;
		return (0);
	}
	if (access(home, F_OK) != 0)
	{
		printf("-minishell: %s: No such file or directory\n", home);
		main->last_exit_status = 1;
		return (0);
	}
	if (chdir(home) != 0)
	{
		printf("-minishell: Cannot enter into the folder.\n");
		main->last_exit_status = 1;
		return (0);
	}
	changepwd(main);
	main->last_exit_status = 0;
	return (1);
}

int	cd_error_cases(t_main *main, int total_args, const char *path)
{
	if (total_args > 2)
	{
		printf("-minishell: Too many arguments");
		main->last_exit_status = 1;
		return (0);
	}
	if (access(path, F_OK) != 0 && total_args == 2)
	{
		printf("-minishell: %s: No such file or directory\n", path);
		main->last_exit_status = 1;
		return (0);
	}
	if (path && !path[0])
	{
		main->last_exit_status = 0;
		return (0);
	}
	if (chdir(path) != 0 && total_args == 2)
	{
		printf("-minishell: Cannot enter into the folder.\n");
		main->last_exit_status = 1;
		return (0);
	}
	return (1);
}

int	cd(t_main *main, int total_args, const char *path)
{
	if (cd_error_cases(main, total_args, path) == 0)
		return (0);
	if (total_args == 1)
		return (cd_home(main));
	changepwd(main);
	if (main->tube != NULL && main->tube->fd >= 0)
		close(main->tube->fd);
	main->tube = NULL;
	return (1);
}
