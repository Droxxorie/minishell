/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:42:52 by eraad             #+#    #+#             */
/*   Updated: 2025/09/22 17:22:20 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	check_child_process(void)
{
	if (!isatty(STDIN_FILENO))
	{
		ft_putstr_fd("minishell: No child process allowed\n", 2);
		exit(EXIT_FAILURE);
	}
}

static int	empty_line_handler(t_data *data)
{
	int	i;

	i = 0;
	if (!data->line[0])
		return (free(data->line), 1);
	while (ft_isspace(data->line[i]))
		i++;
	if (!data->line[i])
		return (free(data->line), 1);
	if (data->line[0] == ':' && !data->line[1])
		return (free(data->line), 1);
	if (data->line[0] == '!' && !data->line[1])
	{
		data->exit_status = 1;
		return (free(data->line), 1);
	}
	return (0);
}

static void	launch_minishell(t_data *data)
{
	while (1)
	{
		signals_handler();
		g_waiting = 0;
		data->line = readline("minishell$ ");
		if (g_waiting == 1 || g_waiting == 3)
			data->exit_status = 130;
		if (data->line == NULL)
			exit_minishell(data, EXIT_SUCCESS);	// TODO ou exit_status ?
		if (empty_line_handler(data))
			continue ;
		if (syntax_error_handler(data))
			continue ;							//TODO besoin de free ?
		if (ft_strlen(data->line))
			add_history(data->line);
		if (expander(data) == 1 || lexer(data, 1) == 1
			|| parser(data) == 1 || executer(data) == 1)
		{
			reset_command_context(data);
			continue ;
		}
		reset_command_context(data);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	if (argc != 1)
		return (1);
	check_child_process();
	ft_memset(&data, 0, sizeof(t_data));
	if (init(&data, envp) == EXIT_FAILURE)
		exit_minishell(&data, EXIT_FAILURE); // TODO
	launch_minishell(&data);                 // TODO
	return (EXIT_SUCCESS);
}
