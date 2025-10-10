/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argv_builder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:39:09 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 18:32:28 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	append_flags_to_argv(t_command *current, size_t *i)
{
	t_minilist	*flags;

	if (!current || !i)
		return (EXIT_FAILURE);
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
	if (!current || !current->argv || !i)
		return (EXIT_FAILURE);
	if (!current->command)
		return (EXIT_SUCCESS);
	current->argv[*i] = ft_strdup(current->command);
	if (!current->argv[*i])
	{
		report_error(NULL, "strdup", -1);
		return (EXIT_FAILURE);
	}
	(*i)++;
	return (EXIT_SUCCESS);
}

int	append_args_to_argv(t_command *current, size_t *i)
{
	t_minilist	*args;

	if (!current || !i)
		return (EXIT_FAILURE);
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

size_t	count_command_elements(t_command *command)
{
	size_t		count;
	t_minilist	*args;
	t_minilist	*flags;

	if (!command)
		return (0);
	if (!command->command)
		count = 0;
	else
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
