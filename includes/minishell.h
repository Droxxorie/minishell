/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:25:02 by eraad             #+#    #+#             */
/*   Updated: 2025/06/23 14:53:13 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//* ----------------- Includes ----------------------- *
#include "../libft/include/libft.h"

//* ----------------- Structs ------------------------ *
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_OUT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ENV_VAR,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection
{
	t_token_type	type;
	char *target;
	struct s_redirection *next;
}	t_redirection;

typedef struct s_command
{
	char **argv;
	t_redirection *redir;
	struct s_command *next;
} t_command;

//* ----------------- Lexer ----------------- *
char *ft_strndup(const char *s, size_t n);
t_token *lexer(const char *input);
t_token *new_token(t_token_type type, const char *value);
const char *handle_word(const char *input, t_token **head, t_token **tail);
const char *handle_pipe(const char *input, t_token **head, t_token **tail);
const char *handle_quote(const char *input, t_token **head, t_token **tail);
const char *handle_dquote(const char *input, t_token **head, t_token **tail);
const char *handle_variable(const char *input, t_token **head, t_token **tail);
const char *handle_reddirection(const char *input, t_token **head, t_token **tail);
void append_token(t_token **head, t_token **tail, t_token_type type, const char *value);

//* ----------------- Parser ----------------- *
char **parse_argv(t_token **tokens);
t_redirection *parser_redirection(t_token **tokens);



#endif