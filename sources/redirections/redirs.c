/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:45:44 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 04:00:18 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_heredoc(t_data *data, t_command *command,
		t_token *limiter_token)
{
	t_redir	*redir;

	redir = new_redir(HEREDOC, limiter_token->quote, limiter_token->value);
	if (!redir)
		return (report_error(NULL, "malloc", -1), EXIT_FAILURE);
	if (setup_heredoc_node(data, redir) == EXIT_FAILURE)
	{
		free(redir->value);
		free(redir);
		return (EXIT_FAILURE);
	}
	redir_push_back(&command->redirs, redir);
	return (EXIT_SUCCESS);
}

void	redir_push_back(t_redir **list, t_redir *node)
{
	t_redir	*current;

	if (!node)
		return ;
	if (!*list)
	{
		*list = node;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = node;
}

t_redir	*new_redir(t_type type, t_quote quote, const char *value)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->quote = quote;
	redir->fd = -1;
	redir->value = NULL;
	if (value)
	{
		redir->value = ft_strdup(value);
		if (!redir->value)
		{
			free(redir);
			return (NULL);
		}
	}
	redir->next = NULL;
	return (redir);
}

int	push_redir(t_data *data, t_command *command, t_token *token)
{
	t_token	*next;
	t_redir	*redir;

	if (!data || !command || !token)
		return (EXIT_FAILURE);
	if (token->type != REDIR_IN && token->type != REDIR_OUT
		&& token->type != REDIR_APPEND && token->type != HEREDOC)
		return (EXIT_FAILURE);
	next = token->next;
	if (!next || next->type == PIPE || next->type == REDIR_IN
		|| next->type == REDIR_OUT || next->type == REDIR_APPEND
		|| next->type == HEREDOC)
		return (report_error2("syntax error near unexpected token:",
				token->value), EXIT_FAILURE);
	if (token->type == HEREDOC)
	{
		next->type = FILE_NAME;
		return (handle_heredoc(data, command, next));
	}
	redir = new_redir(token->type, next->quote, next->value);
	if (!redir)
		return (report_error(NULL, "malloc", -1), EXIT_FAILURE);
	next->type = FILE_NAME;
	redir_push_back(&command->redirs, redir);
	return (EXIT_SUCCESS);
}
