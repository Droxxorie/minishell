/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:59:44 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 16:03:50 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/**
 * @brief Appends a new token to the end of the linked list of tokens.
 * 
 * @param head Pointer to the head of the token list.
 * @param tail Pointer to the tail of the token list.
 * @param type The type of the token to append.
 * @param value The value of the token to append.
 */
void append_token(t_token **head, t_token **tail, t_token_type type, const char *value)
{
	t_token *new;

	new = new_token(type, value);
	if (!new)
		return; //TODO: Handle memory allocation error
	if (*head == NULL)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}
