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

int	g_signal = 0;

void	anti_leaks(t_main **main_struct)
{
	t_envp			*tmp;
	t_history		*tmp2;

	while ((*main_struct)->datas != NULL)
	{
		tmp = (*main_struct)->datas;
		(*main_struct)->datas = (*main_struct)->datas->next;
		free(tmp);
	}
	while ((*main_struct)->history != NULL)
	{
		tmp2 = (*main_struct)->history;
		(*main_struct)->history = (*main_struct)->history->next;
		free(tmp2->cmd);
		free(tmp2);
	}
	rl_clear_history();
	free((*main_struct));
	exit(0);
}

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = SIGINT;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_main		*main_struct;

	signal(SIGINT, handle_signal);
	main_struct = ft_calloc(1, sizeof(t_main));
	if (main_struct == NULL)
		return (0);
	main_struct->datas = list_maker(envp);
	main_struct->cmds_paths = cmds_paths_maker();
	(void)argc;
	(void)argv;
	line_reader(main_struct);
	free(main_struct);
	return (1);
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
