/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_H
# define CMDS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h> 
# include <sys/stat.h>
# include "../libft/libft.h"

typedef struct s_export {
	char			*data;
	struct s_export	*next;
}	t_export;

t_export	*add_cell(char *str);
int			args_checker(int argc);
t_export	*list_maker(int argc, char **argv, char **envp);
char		*data_spliter(char	*str);
void		export(int argc, char **argv, t_export **datas);
void		unset(int argc, char **argv, t_export **datas);

#endif
