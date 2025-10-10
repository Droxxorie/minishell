/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:08:27 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 19:08:08 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	cleanup_shell_state(t_data *data)
{
	if (!data)
		return ;
	reset_command_context(data);
	free_env_list(&data->env_copy);
	free_env_list(&data->export);
	free_tokens(data);
	free_commands(data);
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
	rl_clear_history();
	exit(status);
}
