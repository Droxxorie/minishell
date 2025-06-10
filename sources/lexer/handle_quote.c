/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:29:33 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 19:29:54 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Extracts a single-quoted string from the input.
 * This function scans the input string for a single-quoted string, starting from the current position.
 * It copies the characters between the opening and closing single quotes into a newly allocated string.
 * If the closing quote is not found, it returns NULL.
 * 
 * @param input Pointer to the input string. The pointer will be updated to point to the character after the closing quote.
 * 
 * @return A newly allocated string containing the extracted single-quoted string. The caller is responsible for freeing this string.
 */
static char *extract_quote(const char **input)
{
	const char *start;
	char *result;
	
	(*input)++;
	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	result = ft_strndup(start, *input - start);
	if (**input == '\'')
		(*input)++;
	else
	{
		free(result);
		result = NULL;
	}
	return (result);
}

/**
 * @brief Handles a single-quoted string in the input and appends it as a token.
 * This function extracts a single-quoted string from the input and appends it as a TOKEN_WORD to the linked list of tokens.
 * It updates the input pointer to point to the character after the closing quote.
 * If the closing quote is not found, it returns NULL to indicate an error.
 * 
 * @param input Pointer to the input string. The pointer will be updated to point to the character after the closing quote.
 * @param head Pointer to the head of the token list.
 * @param tail Pointer to the tail of the token list.
 * 
 * @return The updated input pointer after processing the quoted string, or NULL if an error occurs.
 */
const char *handle_quote(const char *input, t_token **head, t_token **tail)
{
	char *word;

	word = extract_quote(&input);
	if (!word)
	{
		//TODO: ERROR
		return (NULL);
	}
	append_token(head, tail, TOKEN_WORD, word);
	free(word);
	return (input);
}
