/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:36:14 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:39:03 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_token(t_data *data, t_token *tok, t_command **cur,
		int *boundary)
{
	t_command	*before;

	if (tok->type == PIPE)
	{
		*boundary = 1;
		*cur = NULL;
	}
	else if (tok->type == CMD || tok->type == ARG || tok->type == FLAG)
	{
		if (dispatch_command_tokens(data, cur, tok, boundary) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (tok->type == REDIR_IN || tok->type == REDIR_OUT
		|| tok->type == REDIR_APPEND || tok->type == HEREDOC)
	{
		before = *cur;
		if (handle_redirection_token(data, cur, tok) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (before == NULL && *cur != NULL)
			*boundary = 0;
	}
	return (EXIT_SUCCESS);
}

static int	finalize_parser(t_data *data, int boundary)
{
	if (boundary && *data->line)
	{
		report_error(data, "No command after pipe", -1);
		return (EXIT_FAILURE);
	}
	if (build_command_argv(data) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	parser(t_data *data)
{
	t_token		*tok;
	int			boundary;
	t_command	*cur;

	tok = data->tokens;
	boundary = 1;
	cur = NULL;
	while (tok)
	{
		if (handle_token(data, tok, &cur, &boundary) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		tok = tok->next;
	}
	return (finalize_parser(data, boundary));
}
