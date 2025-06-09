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

int signalll = 0;

void	anti_leaks(t_main **main_struct)
{
	t_envp			*tmp;
	int				i;

	i = 0;
	while ((*main_struct)->datas != NULL)
	{
		tmp = (*main_struct)->datas;
		(*main_struct)->datas = (*main_struct)->datas->next;
		free(tmp->data);
		free(tmp);
	}
	while ((*main_struct)->cmds_paths->paths[i] != NULL)
	{
		printf("CLEARING %s\n", (*main_struct)->cmds_paths->paths[i]); // Penser à free l'historique aussi
		free((*main_struct)->cmds_paths->paths[i]);
		i++;
	}
	//printf("CLEARING %s\n", (*main_struct)->cmds_paths->paths[i]);
	//free((*main_struct)->cmds_paths->paths[i]);
	free((*main_struct)->cmds_paths->paths);
	free((*main_struct)->cmds_paths);
	free((*main_struct));
	exit(0);
}

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		signalll = SIGINT;
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
	main_struct = malloc(sizeof(t_main));
	if (main_struct == NULL)
		return (0);
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
	line_reader(main_struct);
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
