/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cduquair <cduquair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

extern int	g_signal;

void	handle_signal_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		rl_done = 1;
		ioctl(STDOUT_FILENO, TIOCSTI, "");
		g_signal = SIGINT;
	}
}

void	set_heredoc_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

void	reset_signal(void)
{
	struct sigaction	sa;

	signal(SIGINT, SIG_DFL);
	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	heredoc_interrupt(char **line, int tube[2])
{
	if (g_signal == SIGINT)
	{
		if (line)
			free(*line);
		close(tube[0]);
		close(tube[1]);
		g_signal = 0;
		reset_signal();
		return (-1);
	}
	if (!*line)
	{
		return (reset_signal(), close(tube[1]), 0);
	}
	return (1);
}

int	ft_heredoc(t_main *main, char *end)
{
	char	*line;
	int		tube[2];

	g_signal = 0;
	set_heredoc_signal();
	if (pipe(tube) == -1)
		return (reset_signal(), -1);
	while (1)
	{
		line = readline("> ");
		parse_heredoc_env(main->envp, &line, main->last_exit_status);
		if (heredoc_interrupt(&line, tube) == -1)
			return (-1);
		if (heredoc_interrupt(&line, tube) == 0)
			break ;
		if (ft_strcmp(line, end) == 0)
		{
			free(line);
			break ;
		}
		write(tube[1], line, ft_strlen(line));
		write(tube[1], "\n", 1);
		free(line);
	}
	return (close(tube[1]), reset_signal(), tube[0]);
}
