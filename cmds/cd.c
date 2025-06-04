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

#include "cmds.h"

int	cd(int total_args, const char *path)
{
	if (total_args > 2)
	{
		printf("-bash: cd: too many arguments");
		return (0);
	}
	if (!path)
	{
		printf("Empty path provided.\n");
		return (0);
	}
	if (chdir(path) != 0)
	{
		printf("Cannot enter into the folder.\n");
		return (0);
	}
	return (1);
}
