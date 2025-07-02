/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:41:35 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/02 13:41:42 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_redir
{
	t_token_type		type;
	char				*filename;
	char				*content;
	int					fd;
	struct s_redir		*next;
}	t_redir;

typedef struct s_cmd_info
{
	t_redir				*redirs;
	char				*cmd;
	int					argc;
	char				**argv;
	int					infile_fd;
	int					outfile_fd;
	struct s_cmd_info	*next;
}	t_cmd_info;

char		*rm_quotes(const char *str, size_t start, size_t n);
t_cmd_info	*parse_tokens(t_token *tokens);
t_cmd_info	*create_cmd_info(char *cmd, int argc);
void		append_cmd_info(t_cmd_info **head, t_cmd_info *new);
int			count_cmd_args(t_token *tokens);
t_cmd_info	*parse_tokens(t_token *tokens);
int			check_tokens(t_token *tokens);
t_redir		*create_redir(char *filename, char *content, t_token_type type);
void		append_redir(t_redir **head, t_redir *new);

#endif