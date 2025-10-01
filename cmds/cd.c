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

int	changepwd(t_main *main, int onlyonecommand)
{
	char	*buffer;

	buffer = malloc(1024 * sizeof(char));
	if (getcwd(buffer, 1024) == NULL)
		return (free(buffer), -1);
	if (onlyonecommand == 1)
		add_or_replace(main, &main->envp, ft_strdup("PWD"), buffer);
	else
		free(buffer);
	return (1);
}

int	cd_home(t_main *main, int onlyonecommand)
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
	changepwd(main, onlyonecommand);
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
	if (path && path[0] && access(path, F_OK) != 0 && total_args == 2)
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
	if (path && path[0] && chdir(path) != 0 && total_args == 2)
	{
		printf("-minishell: Cannot enter into the folder.\n");
		main->last_exit_status = 1;
		return (0);
	}
	return (1);
}

int	cd(t_main *main, int total_args, const char *path, int onlyonecommand)
{
	printf("CDDDDDDDDDDDDDDDDDDDDDDDd\n");
	if (cd_error_cases(main, total_args, path) == 0)
		return (0);
	if (total_args == 1)
		return (cd_home(main, onlyonecommand));
	changepwd(main, onlyonecommand);
	if (main->tube != NULL && main->tube->fd >= 0)
		close(main->tube->fd);
	return (1);
}
