/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:41:05 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 14:43:17 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

volatile sig_atomic_t	g_waiting = 0;

static int	empty_line_handler(t_data *data)
{
	int	i;

	i = 0;
	if (!data->line[0])
		return (free(data->line), 1);
	while (ft_iswhitespace(data->line[i]))
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
		reset_command_context(data);
		data->line = readline("minishell$ ");
		if (g_waiting == 1)
			data->exit_status = 130;
		else if (g_waiting == 3)
			data->exit_status = 131;
		if (data->line == NULL)
			exit_minishell(data, data->exit_status);
		if (empty_line_handler(data))
			continue ;
		if (syntax_error_handler(data))
		{
			reset_command_context(data);
			continue ;
		}
		if (ft_strlen(data->line))
			add_history(data->line);
		if (expander(data) == EXIT_FAILURE || lexer(data) == EXIT_FAILURE
			|| parser(data) == EXIT_FAILURE || executor(data) == EXIT_FAILURE)
		{
			reset_command_context(data);
			continue ;
		}
		reset_command_context(data);
	}
}

void	cleanup_shell_state(t_data *data)
{
	if (!data)
		return ;
	free_env_list(&data->env_copy);
	free_env_list(&data->export);
	free_tokens(data);
	free_commands(data);
	data->line = NULL;
	free(data->path);
	data->path = NULL;
	free_pipes_all(data);
}

void	exit_minishell(t_data *data, int exit_status)
{
	int	status;

	status = exit_status;
	write(STDOUT_FILENO, "exit\n", 5);
	close_fds_from(3);
	if (data)
		cleanup_shell_state(data);
	// rl_clear_history(); //? ou clear_history() ?
	clear_history();
	exit(status);
}

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
