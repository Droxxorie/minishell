/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argv_builder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:07:22 by eraad             #+#    #+#             */
/*   Updated: 2025/10/04 19:03:09 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	count_command_elements(t_command *command)
{
	size_t	count;
	t_minilist	*args;
	t_minilist	*flags;

	count = 1;
	args = command->args;
	flags = command->flags;
	while (args)
	{
		count++;
		args = args->next;
	}
	while (flags)
	{
		count++;
		flags = flags->next;
	}
	return (count);
}

int	append_args_to_argv(t_command *current, size_t *i)
{
	t_minilist	*args;

	args = current->args;
	while (args)
	{
		current->argv[*i] = ft_strdup(args->content);
		if (!current->argv[*i])
		{
			perror("minishell: strdup");
			return (EXIT_FAILURE);
		}
		(*i)++;
		args = args->next;
	}
	return (EXIT_SUCCESS);
}

int	append_flags_to_argv(t_command *current, size_t *i)
{
	t_minilist	*flags;

	flags = current->flags;
	while (flags)
	{
		current->argv[*i] = ft_strdup(flags->content);
		if (!current->argv[*i])
		{
			perror("minishell: strdup");
			return (EXIT_FAILURE);
		}
		(*i)++;
		flags = flags->next;
	}
	return (EXIT_SUCCESS);
}

int	append_command_to_argv(t_command *current, size_t *i)
{
	current->argv[*i] = ft_strdup(current->command);
	if (!current->argv[*i])
	{
		perror("minishell: strdup");
		return (EXIT_FAILURE);
	}
	(*i)++;
	return (EXIT_SUCCESS);
}
