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

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*user_input;
	t_main		*main_struct;

	main_struct = malloc(sizeof(t_main));
	main_struct->datas = list_maker(envp);
	main_struct->cmds_paths = cmds_paths_maker();
	int	i;
	i = 0;
	while (main_struct->cmds_paths->paths[i])
	{
		printf("%s\n", main_struct->cmds_paths->paths[i]);
		i++;
	}
	(void)argc;
	(void)argv;
	while (1)
	{
		user_input = readline(PURPLE "minishell>" RESET);
		if (check_cmds(user_input, &main_struct->datas) == 0)
			cmd_searcher(user_input, main_struct->cmds_paths->paths);
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
