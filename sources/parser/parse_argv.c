/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:29:32 by eraad             #+#    #+#             */
/*   Updated: 2025/06/11 17:29:34 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **parse_argv(t_token **tokens)
{
	char **argv;
	int argc;

	argv = malloc(sizeof(char *) * 1024); //TODO: defnir dynamiquement
	argc = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD || (*tokens)->type == TOKEN_ENV_VAR)
	{
		argv[argc++] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	argv[argc] = NULL;
	return (argv);
}
