/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:31:29 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 19:32:03 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles a pipe token in the input string.
 * This function checks if the current character is a pipe ('|') and appends a TOKEN_PIPE to the linked list of tokens.
 * It updates the input pointer to point to the next character after the pipe.
 * If the pipe character is found, it appends a new token with type TOKEN_PIPE and value "|".
 * If the pipe character is not found, it simply returns the input pointer unchanged.
 * 
 * @param input A pointer to the current position in the input string.
 * @param head A pointer to the head of the linked list of tokens.
 * @param tail A pointer to the tail of the linked list of tokens.
 * 
 * @return The updated input string after the pipe character.
 */
const char *handle_pipe(const char *input, t_token **head, t_token **tail)
{
	append_token(head, tail, TOKEN_PIPE, "|");
	return (input + 1);
}
