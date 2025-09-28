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

static t_main	*init_main(char **envp)
{
	t_main	*main_struct;

	main_struct = ft_calloc(1, sizeof(t_main));
	if (!main_struct)
		exit(1);
	main_struct->envp = init_env(envp);
	return (main_struct);
}

int	main(int argc, char **argv, char **envp)
{
	t_main				*main_struct;
	struct sigaction	sa;

	if (argc >= 2)
	{
		printf("Pas d'arguments stp !\n");
		return (1);
	}
	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	main_struct = init_main(envp);
	(void)argv;
	line_reader(main_struct);
	free_main(&main_struct);
}
