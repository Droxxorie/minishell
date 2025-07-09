/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:09:57 by eraad             #+#    #+#             */
/*   Updated: 2025/06/11 17:49:46 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token *parser_next_command(t_token *tokens, t_command **head, t_command **tail)
{
	t_command *command;
	
	command = malloc(sizeof(t_command));
	if (!command)
		return NULL; //TODO handle error
	command->argv = parser_argv(&tokens);
	command->redir = parser_redirection(&tokens);
	command->next = NULL;
	if (*head == NULL)
		*head = command;
	else
		(*tail)->next = command;
	*tail = command;
	if (tokens && tokens->type == TOKEN_PIPE)
		tokens = tokens->next;
	return tokens;
}
t_command *parser(t_token *tokens)
{
	t_command *head;
	t_command *tail; //? next

	head = NULL;
	tail = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
		tokens = parser_next_command(tokens, &head, &tail);

	return head;
}
