/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:48:35 by eraad             #+#    #+#             */
/*   Updated: 2025/10/01 17:37:00 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static void	print_echo_args(char **argv, int index, t_bool has_n_flag)
{
	t_bool	first_arg;

	if (!argv || !argv[index])
	{
		if (has_n_flag == FALSE)
			printf("\n");
		return ;
	}
	first_arg = TRUE;
	while (argv[index])
	{
		if (argv[index][0] == '\0')
		{
			index++;
			continue ;
		}
		if (first_arg == FALSE)
			printf(" ");
		first_arg = FALSE;
		printf("%s", argv[index]);
		index++;
	}
	if (has_n_flag == FALSE)
		printf("\n");
}

static t_bool	is_echo_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (FALSE);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	execute_builtin_echo(char **argv)
{
	int		index;
	t_bool	has_n_flag;

	index = 1;
	has_n_flag = FALSE;
	while (argv[index])
	{
		if (is_echo_n_flag(argv[index]) == TRUE)
		{
			has_n_flag = TRUE;
			index++;
		}
		else
			break ;
	}
	print_echo_args(argv, index, has_n_flag);
	return (EXIT_SUCCESS);
}
