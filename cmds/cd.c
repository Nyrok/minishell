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

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: ./cd <path>");
		return (EXIT_FAILURE);
	}
	if (chdir(argv[1]) != 0)
	{
		printf("Cannot enter into the folder.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
