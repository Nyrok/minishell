/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:51:55 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/09 16:52:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_info	*create_cmd_info(char *cmd, int argc)
{
	t_cmd_info	*cmd_info;

	cmd_info = ft_calloc(1, sizeof(t_cmd_info));
	if (!cmd_info)
		return (NULL);
	if (!cmd)
		return (cmd_info);
	cmd_info->cmd = ft_strdup(cmd);
	cmd_info->argc = cmd != NULL;
	cmd_info->argv = ft_calloc(argc + 1, sizeof(char *));
	if (!cmd_info->argv)
		return (NULL);
	cmd_info->argv[0] = ft_strdup(cmd);
	cmd_info->argv[argc] = NULL;
	return (cmd_info);
}

void	append_cmd_info(t_cmd_info **head, t_cmd_info *new)
{
	t_cmd_info	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	count_cmd_args(t_token *tokens)
{
	int	i;

	i = 1;
	while (tokens && tokens->type == WORD)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}
