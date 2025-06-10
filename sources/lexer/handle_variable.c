/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:10:34 by eraad             #+#    #+#             */
/*   Updated: 2025/06/10 19:32:46 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Extracts a variable from the input string.
 * This function identifies a variable in the input string, which starts with a '$' character.
 * If the variable is a special case (like '$?'), it handles it accordingly.
 * Otherwise, it extracts the variable name, which can consist of alphanumeric characters and underscores.
 * The function updates the input pointer to point to the next character after the variable.
 * 
 * 
 * @param input Pointer to the input string, which will be updated to point to the next character after the variable.
 * 
 * @return A dynamically allocated string containing the extracted variable.
 */
static char *extract_variable(const char **input)
{
	const char *start;
	char *result;

	(*input)++;
	if (**input == '?')
	{
		result = ft_strdup('?');
		(*input)++;
	}
	else
	{
		start = *input;
		while (**input && (ft_isalnum(**input) || **input == '_'))
			(*input)++;
		result = ft_strndup(start, *input - start);
	}
	//TODO: Handle memory allocation errors
	return result;
}

/**
 * @brief This function extracts a variable from the input string, creates a new token of type TOKEN_ENV_VAR, and appends it to the token list.
 * It updates the input pointer to point to the next character after the variable.
 * This function is used in the lexer to handle environment variables in the input string.
 * 
 * @param input Pointer to the input string, which will be updated to point to the next character after the variable.
 * @param head Pointer to the head of the token list.
 * @param tail Pointer to the tail of the token list.
 * 
 * @return The updated input string after processing the variable.
 */
const char *handle_variable(const char *input, t_token **head, t_token **tail)
{
	char *word;

	word = extract_variable(&input);
	if (!word)
	{
		//TODO: Handle error
		return NULL;
	}
	append_token(head, tail, TOKEN_ENV_VAR, word);
	free(word);
	return input;
}
