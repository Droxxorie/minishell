/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:42:52 by eraad             #+#    #+#             */
/*   Updated: 2025/09/15 16:49:05 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void check_child_process(void)
{
	if (!isatty(STDIN_FILENO))
	{
		ft_putstr_fd("minishell: No child process allowed\n", 2);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_data data;

	(void)argv;
	if (argc != 1)
		return (1);
	check_child_process();
	ft_memset(&data, 0, sizeof(t_data));
	if (init(&data, envp) == EXIT_FAILURE)
		exit_minishell(&data, EXIT_FAILURE); //TODO
	launch_minishell(&data); //TODO
	return (EXIT_SUCCESS);
}
