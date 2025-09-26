/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_paths_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_cmds_paths	*cmds_paths_maker(void)
// {
// 	static t_cmds_paths	cmds_paths;
// 	static char			*paths[] = {
// 		"", // Je lai add pour les trucs genre /bin/ls
// 		"/bin",
// 		"/sbin",
// 		"/usr/bin",
// 		"/usr/sbin",
// 		"/usr/local/bin", // Adapter ca aux env
// 		"/usr/local/sbin",
// 		"/snap/bin",
// 		NULL
// 	};

// 	cmds_paths.paths = paths;
// 	return (&cmds_paths);
// }


void	cmds_paths_maker(t_main *main)
{
	//t_envp	**envp;
	char	*env_value;
	int		i;

	i = 0;
	if (main->cmds_paths && main->cmds_paths->paths)
	{
		while (main->cmds_paths->paths[i])
			free(main->cmds_paths->paths[i++]);
		free(main->cmds_paths->paths);
	}
	if (!main->cmds_paths)
		main->cmds_paths = ft_calloc(1, sizeof(t_cmds_paths));
	env_value = get_env_value(main->envp, "PATH");
	if (env_value == NULL)
	{
		main->cmds_paths->paths = NULL;
		return ;
	}
	main->cmds_paths->paths = ft_split(env_value, ':');
}
