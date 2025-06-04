/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cmds/cmds.h"

int	main(void)
{
	char	*user_input;
	while (1)
	{
		user_input = readline("minishell>");
		check_cmds(user_input);
		free(user_input);
	}
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_export	*datas;

// 	datas = list_maker(envp);
// 	env(datas);
// 	if (argc == 1 || envp == NULL)
// 		return (-1);
// 	printf("\n\n\n===EXPORT===\n\n\n");
// 	export(argc, argv, &datas);
// 	env(datas);
// 	argv[1] = "LOL";
// 	argv[2] = "LOL2";
// 	argv[3] = "WSL2_GUI_APPS_ENABLED";
// 	printf("\n\n\n===UNSET===\n\n\n");
// 	unset(argc, argv, &datas);
// 	env(datas);
// }
