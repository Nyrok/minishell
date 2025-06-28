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

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

typedef enum e_type
{
    INVALID = 0,
    WORD,    // ls
    PIPE,    // |
    REDIN,   // < input
    HEREDOC, // << SQL ... SQL
    REDOUT,  // > output
    APPEND,  // >> output
    END
}	t_token_type;

typedef struct s_token
{
    char			*word;
    t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_info_cmd
{
    char				*infile;
	int					fd_infile;
    char				*outfile;
	int					fd_outfile;
	char				**args;
	struct s_info_cmd	*next;
}	t_info_cmd;


t_token	*tokenize_input(const char *input);

#endif