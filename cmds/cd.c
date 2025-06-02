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

int	cd(const char *path)
{
	if (!path)
	{
		printf("Empty path provided.");
		return (0);
	}
	if (chdir(path) != 0)
	{
		printf("Cannot enter into the folder.");
		return (0);
	}
	return (1);
}
