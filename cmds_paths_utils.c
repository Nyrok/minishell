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

t_cmds_paths	*cmds_paths_maker(void)
{
	t_cmds_paths	*cmds_paths;

	cmds_paths = malloc(sizeof(t_cmds_paths));
	cmds_paths->paths = malloc(8 * sizeof(char *));
	if (cmds_paths->paths == NULL)
		return (NULL);
	cmds_paths->paths[0] = ft_strdup("/bin");
	cmds_paths->paths[1] = ft_strdup("/sbin");
	cmds_paths->paths[2] = ft_strdup("/usr/bin");
	cmds_paths->paths[3] = ft_strdup("/usr/sbin");
	cmds_paths->paths[4] = ft_strdup("/usr/local/bin");
	cmds_paths->paths[5] = ft_strdup("/usr/local/sbin");
	cmds_paths->paths[6] = ft_strdup("/snap/bin");
	cmds_paths->paths[7] = NULL;
	return (cmds_paths);
}
