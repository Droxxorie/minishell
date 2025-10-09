/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:08:55 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:14:29 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

static int	read_and_prep_prompt(t_data *data)
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
	{
		exit_minishell(data, data->exit_status);
		return (0);
	}
	return (1);
}

static int	preprocess_line(t_data *data)
{
	if (empty_line_handler(data))
		return (0);
	if (syntax_error_handler(data))
	{
		reset_command_context(data);
		return (0);
	}
	if (ft_strlen(data->line))
		add_history(data->line);
	return (1);
}

static int	execute_pipeline(t_data *data)
{
	if (expander(data) == EXIT_FAILURE || lexer(data) == EXIT_FAILURE
		|| parser(data) == EXIT_FAILURE || executor(data) == EXIT_FAILURE)
	{
		reset_command_context(data);
		return (0);
	}
	reset_command_context(data);
	return (1);
}

void	launch_minishell(t_data *data)
{
	while (1)
	{
		if (!read_and_prep_prompt(data))
			return ;
		if (!preprocess_line(data))
			continue ;
		if (!execute_pipeline(data))
			continue ;
	}
}
