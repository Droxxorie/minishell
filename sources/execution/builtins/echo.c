/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:55:18 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 16:06:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	print_echo_args(char **argv, int index, t_bool has_n_flag)
{
	t_bool	first_arg;

	first_arg = TRUE;
	while (argv[index])
	{
		if (first_arg == FALSE)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, argv[index], ft_strlen(argv[index]));
		first_arg = FALSE;
		index++;
	}
	if (!has_n_flag)
		write(STDOUT_FILENO, "\n", 1);
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
	while (argv[index] && is_echo_n_flag(argv[index]) == TRUE)
	{
		has_n_flag = TRUE;
		index++;
	}
	print_echo_args(argv, index, has_n_flag);
	return (EXIT_SUCCESS);
}
