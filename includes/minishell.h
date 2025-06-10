/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:25:02 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 19:39:32 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//* ----------------- Includes ----------------------- *
#include "../libft/libft.h"

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

//* ----------------- Lexer ----------------- *
char	*ft_strndup(const char *s, size_t n)

t_token *lexer(const char *input);
t_token *new_token(t_token_type type, const char *value);
const char *handle_word(const char *input, t_token **head, t_token **tail);
const char *handle_pipe(const char *input, t_token **head, t_token **tail);
const char *handle_quote(const char *input, t_token **head, t_token **tail);
const char *handle_dquote(const char *input, t_token **head, t_token **tail);
const char *handle_variable(const char *input, t_token **head, t_token **tail);
const char *handle_reddirection(const char *input, t_token **head, t_token **tail);
void append_token(t_token **head, t_token **tail, t_token_type type, const char *value);

#endif