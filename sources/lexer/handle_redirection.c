/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:32:58 by eraad             #+#    #+#             */
/*   Updated: 2025/06/11 16:53:46 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles redirection tokens in the input string.
 * This function checks if the current character is a redirection operator
 * ('<' or '>') and appends the corresponding token to the linked list of tokens.
 * If the operator is '<<' or '>>', it appends a heredoc or append redirection token.
 * 
 * @param input Pointer to the current position in the input string.
 * @param head Pointer to the head of the token linked list.
 * @param tail Pointer to the tail of the token linked list.
 * 
 * @return The updated position in the input string after processing the redirection.
 */
const char *handle_reddirection(const char *input, t_token **head, t_token **tail)
{
	if (*input == '<')
	{
		if (*(input + 1) == '<')
		{
			append_token(head, tail, TOKEN_HEREDOC, "<<");
			return (input + 2);
		}
		append_token(head, tail, TOKEN_REDIRECT_IN, "<");
		return (input + 1);
	}
	else if (*input == '>')
	{
		if (*(input + 1) == '>')
		{
			append_token(head, tail, TOKEN_REDIRECT_OUT_APPEND, ">>");
			return (input + 2);
		}
		append_token(head, tail, TOKEN_REDIRECT_OUT, ">");
		return (input + 1);
	}
	return (NULL);
}
