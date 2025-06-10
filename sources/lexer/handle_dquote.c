/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dquote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:28:53 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 19:28:55 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Extracts a double-quoted string from the input.
 * This function searches for a double-quote character (") in the input string,
 * and extracts the substring between the opening and closing double quotes.
 * It updates the input pointer to point to the character after the closing quote.
 * 
 * @param input Pointer to the input string. The pointer will be updated to point to the character after the closing quote.
 * 
 * @return A newly allocated string containing the extracted double-quoted string. The caller is responsible for freeing this string.
 */
static char *extract_dquote(const char **input)
{
	const char *start;
	char *result;
	
	(*input)++;
	start = *input;
	while (**input && **input != '\"')
		(*input)++;
	result = ft_strndup(start, *input - start);
	if (**input == '\"')
		(*input)++;
	else
	{
		free(result);
		result = NULL;
	}
	return (result);
}

/**
 * @brief Handles a double-quoted string in the input and appends it as a token.
 * This function extracts a double-quoted string from the input,
 * creates a token for it, and appends the token to the linked list of tokens.
 * If the extraction fails (e.g., if there is no closing quote), it returns NULL.
 * 
 * @param input Pointer to the input string. The pointer will be updated to point to the character after the closing quote.
 * @param head Pointer to the head of the token list.
 * @param tail Pointer to the tail of the token list.
 * 
 * @return The updated input pointer after processing the quoted string, or NULL if an error occurs.
 */
const char *handle_dquote(const char *input, t_token **head, t_token **tail)
{
	char *word;

	word = extract_dquote(&input);
	if (!word)
	{
		//TODO: ERROR
		return (NULL);
	}
	append_token(head, tail, TOKEN_WORD, word);
	free(word);
	return (input);
}
