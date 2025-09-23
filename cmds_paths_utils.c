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
	static t_cmds_paths	cmds_paths;
	static char			*paths[] = {
		"", // Je lai add pour les trucs genre /bin/ls
		"/bin",
		"/sbin",
		"/usr/bin",
		"/usr/sbin",
		"/usr/local/bin", // Adapter ca aux env
		"/usr/local/sbin",
		"/snap/bin",
		NULL
	};

	cmds_paths.paths = paths;
	return (&cmds_paths);
}
