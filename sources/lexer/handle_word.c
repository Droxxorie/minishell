/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:14:15 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 19:31:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Extracts a word from the input string, skipping leading whitespace.
 * This function scans the input string until it encounters a whitespace character
 * or a special character (pipe, redirection, quotes, etc.).
 * It allocates a new string containing the extracted word and updates the input pointer
 * to point to the next character after the word.
 * 
 * @param input Pointer to the input string. The pointer will be updated to point to the next character after the extracted word.
 * 
 * @return A newly allocated string containing the extracted word. The caller is
 */
static char *extract_word(const char **input)
{
	const char *start;

	start = *input;
	while (**input && ft_iswhitespace(**input) && !strchr("|<>\"'$", **input))
		(*input)++;
	return (ft_strndup(start, *input - start));
}

/**
 * @brief Handles a word token in the input string.
 * This function extracts a word from the input string,
 * appends it as a TOKEN_WORD to the linked list of tokens,
 * and frees the allocated memory for the word.
 * 
 * @param input Pointer to the current position in the input string.
 * @param head Pointer to the head of the token linked list.
 * @param tail Pointer to the tail of the token linked list.
 * 
 * @return The updated position in the input string after processing the word.
 */
const char *handle_word(const char *input, t_token **head, t_token **tail)
{
	char *word;

	word = extract_word(&input);
	if (!word)
	{
		//TODO: ERROR
		return (NULL);
	}
	append_token(head, tail, TOKEN_WORD, word);
	free(word);
	return (input);
}
