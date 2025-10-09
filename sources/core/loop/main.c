/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:41:05 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 04:01:40 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

volatile sig_atomic_t	g_waiting = 0;

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	if (argc != 1)
		return (1);
	// if (!isatty(STDIN_FILENO))
	// {
	// 	ft_putstr_fd("minishell: no child process allowed\n", 2);
	// 	exit(EXIT_FAILURE);
	// }
	ft_memset(&data, 0, sizeof(t_data));
	if (init(&data, envp) == EXIT_FAILURE)
		exit_minishell(&data, EXIT_FAILURE);
	launch_minishell(&data);
	if (data.exit_status)
		exit_minishell(&data, data.exit_status);
	exit_minishell(&data, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
