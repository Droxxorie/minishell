/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:29:10 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 19:38:34 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *lexer_next_token(const char *input, t_token **head, t_token **tail)
{
	if (ft_iswhitespace(*input)) //TODO: space ore whitespace ??
		return (input + 1);
	if (*input == '|')
		return (handle_pipe(input, head, tail));
	if (*input == '<' || *input == '>')
		return (handle_reddirection(input, head, tail));
	if (*input == '\'')
		return (handle_quote(input, head, tail));
	if (*input == '\"')
		return (handle_dquote(input, head, tail));
	if (*input == '$')
		return (handle_variable(input, head, tail));
	return (handle_word(input, head, tail));
}

t_token *lexer(const char *input)
{
	t_token *head;
	t_token *tail;

	head = NULL;
	tail = NULL;
	while (*input)
	{
		input = lexer_next_token(input, &head, &tail);
		//TODO: clear list & errors
	}
	append_token(&head, &tail, TOKEN_EOF, NULL);
	return (head);
}
