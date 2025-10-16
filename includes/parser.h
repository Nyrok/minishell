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
# include "free.h"

typedef struct s_redir
{
	t_token_type		type;
	int					io;
	char				*filename;
	int					fd;
	int					good;
	struct s_redir		*next;
}	t_redir;

struct s_cmd_info
{
	t_redir				*redirs;
	char				*cmd;
	int					argc;
	char				**argv;
	char				*cmd_path;
	t_redir				*infile;
	t_redir				*outfile;
	int					tube[2];
	struct s_cmd_info	*next;
};

char		*get_word(const char *str, size_t *i);
t_token		*get_redir_token(const char *str, size_t *i);
t_cmd_info	*create_cmd_info(char *cmd, int argc);
void		append_cmd_info(t_cmd_info **head, t_cmd_info *new);
int			count_cmd_args(t_token *tokens);
t_cmd_info	*parse_tokens(t_main *main, t_token *tokens);
int			check_tokens(t_token *tokens);
t_redir		*create_redir(char *filename, t_token_type type);
void		append_redir(t_redir **head, t_redir *new);
t_token		*create_token(char *word, t_token_type type);
void		setup_cmd_redirs(t_cmd_info *cmd_info);
void		parse_word_env(t_envp *envp, char **word, int last_exit_status);
void		parse_env(t_envp *envp, char **str, int last_exit_status);
char		*three_strjoin(char *old, char *s1, char *s2, char *s3);
char		*rm_nchar(char *str, const char c, size_t n);
char		*rm_dollars(char *str, size_t start, size_t n);
int			count_cmd_info(t_cmd_info *cmd_info);

#endif