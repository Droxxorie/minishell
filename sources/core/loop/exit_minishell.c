/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:08:27 by eraad             #+#    #+#             */
/*   Updated: 2025/10/11 12:49:58 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	cleanup_and_exit(t_data *data, int exit_status)
{
	cleanup_shell_state(data);
	rl_clear_history();
	exit(exit_status);
}

void	cleanup_shell_state(t_data *data)
{
	if (!data)
		return ;
	if (data->line_base)
	{
		free(data->line_base);
		data->line_base = NULL;
	}
	data->line = NULL;
	reset_command_context(data);
	free_env_list(&data->env_copy);
	free_env_list(&data->export);
	if (data->path)
	{
		free(data->path);
		data->path = NULL;
	}
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
	rl_clear_history();
	exit(status);
}
