/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 13:19:04 by eraad             #+#    #+#             */
/*   Updated: 2025/10/02 13:25:12 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static int print_env(t_data *data, int fd_out)
{
	t_env *current;
	
	current = data->env_copy;
	while (current)
	{
		if (current->equal == '=')
		{
			if (safe_putstr_fd(current->key, fd_out) == EXIT_FAILURE
				|| safe_putchar_fd('=', fd_out) == EXIT_FAILURE
				|| safe_putstr_fd(current->value, fd_out) == EXIT_FAILURE
				|| safe_putchar_fd('\n', fd_out) == EXIT_FAILURE)
			{
				report_error(data, "write", -1);
				return (EXIT_FAILURE);
			}
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int execute_builtin_env(t_data *data, char **argv, int fd_out)
{
	if (!data || !argv)
		return (EXIT_FAILURE);
	if (argv[1] != NULL)
	{
		report_error2("env: ", "too many arguments");
		data->exit_status += 1;
		return (EXIT_FAILURE);
	}
	if (print_env(data, fd_out) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
