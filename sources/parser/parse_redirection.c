/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:29:44 by eraad             #+#    #+#             */
/*   Updated: 2025/06/23 14:55:39 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_redirection *add_redirection(t_redirection *redir, t_token_type type, char *target)
{
	t_redirection *new_redir;

	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
		return NULL; //TODO handle error
	new_redir->type = type;
	new_redir->target = ft_strdup(target);
	new_redir->next = redir;
	return new_redir;
}

t_redirection *parser_redirection(t_token **tokens)
{
	t_redirection *redir;
	t_token_type type;
	
	redir = NULL;
	while (*tokens && ((*tokens)->type == TOKEN_REDIRECT_IN ||
		(*tokens)->type == TOKEN_REDIRECT_OUT ||
		(*tokens)->type == TOKEN_REDIRECT_OUT_APPEND ||
		(*tokens)->type == TOKEN_HEREDOC))
	{
		type = (*tokens)->type;
		*tokens = (*tokens)->next;
		if (!*tokens || (*tokens)->type != TOKEN_WORD)
		{
			//TODO handle error: expected a target after redirection token
			return redir;
		}
		redir = add_redirection(redir, type, (*tokens)->value);
	}
	return redir;
}
