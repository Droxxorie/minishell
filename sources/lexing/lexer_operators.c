/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:33:52 by eraad             #+#    #+#             */
/*   Updated: 2025/10/12 13:26:25 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_type	get_operator_token_type(t_data *data, char operator, int
		*command_boundary)
{
	if (operator == '|')
	{
		*command_boundary = 1;
		return (PIPE);
	}
	else if (operator == '>' && data->line[1] == '>')
	{
		data->line++;
		return (REDIR_APPEND);
	}
	else if (operator == '<' && data->line[1] == '<')
	{
		data->line++;
		return (HEREDOC);
	}
	else if (operator == '>')
		return (REDIR_OUT);
	else if (operator == '<')
		return (REDIR_IN);
	return (-1);
}

static char	*build_remaning_operator_token(t_data *data, t_type *type)
{
	if (*type == PIPE)
	{
		if (data->line && data->line[1] == '|')
			return (data->exit_status = 2, print_syntax_error('|', 4), NULL);
		return (ft_strdup("|"));
	}
	else if (*type == HEREDOC)
	{
		if (data->line && data->line[1] == '<')
			return (data->exit_status = 2, print_syntax_error('<', 4), NULL);
		return (ft_strdup("<<"));
	}
	return (NULL);
}

static char	*build_operator_token(t_data *data, char operator, int
		*command_boundary, t_type *type)
{
	*type = get_operator_token_type(data, operator, command_boundary);
	if (*type == REDIR_APPEND)
	{
		if (data->line && data->line[1] == '>')
			return (data->exit_status = 2, print_syntax_error('>', 4), NULL);
		return (ft_strdup(">>"));
	}
	else if (*type == REDIR_IN)
	{
		if (data->line && data->line[1] == '>')
			return (data->exit_status = 2, print_syntax_error('>', 4), NULL);
		return (ft_strdup("<"));
	}
	else if (*type == REDIR_OUT)
	{
		if (data->line && data->line[1] == '<')
			return (data->exit_status = 2, print_syntax_error('<', 4), NULL);
		return (ft_strdup(">"));
	}
	return (build_remaning_operator_token(data, type));
}

int	add_operator_token(t_data *data, char operator, int *command_boundary)
{
	t_type	type;
	char	*raw_operator;
	t_token	*new_token;

	raw_operator = build_operator_token(data, operator, command_boundary,
			&type);
	if (!raw_operator)
		return (EXIT_FAILURE);
	new_token = create_token(raw_operator, raw_operator
			+ ft_strlen(raw_operator), type, NO_QUOTE);
	if (!new_token)
	{
		free(raw_operator);
		return (EXIT_FAILURE);
	}
	add_back_token(data, new_token);
	free(raw_operator);
	return (EXIT_SUCCESS);
}
