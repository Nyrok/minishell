/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"
# include "fwd.h"

struct s_envp
{
	char			*key;
	char			*value;
	char			*full;
	struct s_envp	*next;
};

t_envp	*create_envp(char *key, char *value);
t_envp	*init_env(char **envp);
void	add_or_replace(t_main *main, t_envp **actual, char *key, char *value);
char	**envp_to_str(t_envp *envp);
int		is_valid_env_char(char c);
int		is_valid_env_name(char *str);
char	*get_env_value(t_envp *envp, char *key);
t_envp	**get_env_addr(t_envp **envp, char *key);

#endif
