/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:42:52 by eraad             #+#    #+#             */
/*   Updated: 2025/10/03 22:54:07 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_shell_state(t_data *data)
{
	int	i;

	if (!data)
		return ;
	free_env_list(&data->env_copy);
	free_env_list(&data->export);
	free_tokens(&data->tokens);
	free_commands(&data->commands);
	free_redirections(data);
	free(data->line);
	data->line = NULL;
	i = 0;
	while (i < 256)
	{
		if (data->heredoc_fds[i] > 2)
			close(data->heredoc_fds[i]);
		data->heredoc_fds[i++] = -1;
	}
	if (data->pipes)
	{
		free(data->pipes->fds);
		free(data->pipes->pids);
		free(data->pipes);
	}
	data->pipes = NULL;
}

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

static void	exit_minishell(t_data *data, int exit_status)
{
	close_fds_from(3);
	if (data->pipes->fds)
	{
		free(data->pipes->fds);
		data->pipes->fds = NULL;
	}
	if (data->pipes->pids)
	{
		free(data->pipes->pids);
		data->pipes->pids = NULL;
	}
	printf("exit\n");
	cleanup_shell_state(data);
	clear_history();
	exit(exit_status);
}

static void	launch_minishell(t_data *data)
{
	while (1)
	{
		signals_handler();
		g_waiting = 0;
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
		if (lexer(data) == EXIT_FAILURE || parser(data) == EXIT_FAILURE
			|| expander(data) == EXIT_FAILURE || executor(data) == EXIT_FAILURE)
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
	if (!isatty(STDIN_FILENO))
	{
		ft_putstr_fd("minishell: no child process allowed\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_memset(&data, 0, sizeof(t_data));
	if (init(&data, envp) == EXIT_FAILURE)
		exit_minishell(&data, EXIT_FAILURE); // TODO
	launch_minishell(&data);
	return (EXIT_SUCCESS);
}
