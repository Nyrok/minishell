/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_cmds_paths
{
	char	**paths;	
}	t_cmds_paths;

typedef enum e_error
{
	NOTFOUND = 0,
	DEF_PIPE,
	FILE_NOT_FOUND,
	ENDED
}	t_error_code;

//void			cmd_searcher(char *cmd, char **paths, t_envp *envp);
t_cmds_paths	*cmds_paths_maker(void);
int				ft_access(char *pathname);

#endif
