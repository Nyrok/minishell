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
		ioctl(STDOUT_FILENO, TIOCSTI, "\n");
		g_signal = SIGINT;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
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

// ...existing code...
int	heredoc_interrupt(char **line, int tube[2])
{
    if (g_signal == SIGINT)
    {
        if (line && *line)
        {
            free(*line);
            *line = NULL;
        }
        /* fermer les deux bouts si interruption */
        close(tube[0]);
        close(tube[1]);
        g_signal = 0;
        reset_signal();
        return (-1);
    }
    /* EOF (Ctrl-D) : readline renvoie NULL -> retourner 0 pour sortir la boucle,
       ne pas fermer les descripteurs ici (ft_heredoc fermera tube[1] une fois) */
    if (line == NULL || *line == NULL)
    {
        return (0);
    }
    return (1);
}

int	ft_heredoc(char *end)
{
    char	*line;
    int		tube[2];
    int		hret;

    set_heredoc_signal();
    if (pipe(tube) == -1)
        return (reset_signal(), -1);
    while (1)
    {
        line = readline("> ");
        hret = heredoc_interrupt(&line, tube);
        if (hret == -1)
            return (-1);
        if (hret == 0)
            break ;
        if (line && ft_strcmp(line, end) == 0)
        {
            free(line);
            break ;
        }
        if (line)
        {
            write(tube[1], line, ft_strlen(line));
            write(tube[1], "\n", 1);
            free(line);
        }
    }
    close(tube[1]);
    reset_signal();
    return (tube[0]);
}
