/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_nodes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:37:37 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 15:38:20 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	append_list_node(t_minilist **list, t_token *token)
{
	t_minilist	*new_node;

	new_node = malloc(sizeof(t_minilist));
	if (!new_node)
	{
		perror("minishell: malloc");
		return (EXIT_FAILURE);
	}
	ft_memset(new_node, 0, sizeof(t_minilist));
	new_node->content = ft_strdup(token->value);
	if (!new_node->content)
	{
		free(new_node);
		perror("minishell: strdup");
		return (EXIT_FAILURE);
	}
	new_node->quote = token->quote;
	if (!*list)
		*list = new_node;
	else
		minilist_add_back(list, new_node);
	return (EXIT_SUCCESS);
}

int	add_command_flag(t_command *current_command, t_token *current_token)
{
	if (!current_command || !current_token)
		return (EXIT_FAILURE);
	if (append_list_node(&current_command->flags,
			current_token) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	add_command_arg(t_command *current_command, t_token *current_token)
{
	if (!current_command || !current_token)
		return (EXIT_FAILURE);
	if (append_list_node(&current_command->args, current_token) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	set_command_name(t_command *current_command, t_token *current_token)
{
	if (!current_command || !current_token)
		return (EXIT_FAILURE);
	current_command->command = ft_strdup(current_token->value);
	if (!current_command->command)
	{
		perror("minishell: strdup");
		return (EXIT_FAILURE);
	}
	current_command->quote = current_token->quote;
	return (EXIT_SUCCESS);
}
