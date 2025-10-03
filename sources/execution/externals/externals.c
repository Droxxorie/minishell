/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:55:49 by eraad             #+#    #+#             */
/*   Updated: 2025/10/03 16:53:31 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	exec_command(t_data *data, t_command *node, char *command_path)
{
	char	**envp;

	if (!data || !node || !command_path)
		return (EXIT_FAILURE);
	envp = env_list_to_array(data->env_copy);
	if (command_path)
		data->exit_status = execve(command_path, node->argv, envp);
	free(envp);
	free(command_path);
	exit(data->exit_status);
}

static void	setup_and_exec_child(t_data *data, t_command *node, int *fds,
		int index, char *command_path)
{
	if (dup2(fds[index - 2], STDIN_FILENO) == -1 || dup2(fds[index + 1],
			STDOUT_FILENO) == -1)
	{
		report_error(data, "dup2", 1);
		free(command_path);
		exit(EXIT_FAILURE);
	}
	close_pipe_fds(fds, data->pipes->nb * 2);
	exec_command(data, node, command_path);
}

static int	fetch_command_at_index(t_data *data, t_command **node, int index)
{
	t_command	*current;
	int			command_count;

	current = data->commands;
	command_count = 0;
	while (current && command_count * 2 < index)
	{
		current = current->next;
		command_count++;
	}
	if (!current || !current->command)
		return (EXIT_FAILURE);
	*node = current;
	return (EXIT_SUCCESS);
}

void	handle_external_command(t_data *data, int *fds, int index, pid_t *pid)
{
	t_command	*node;
	char		*cmd_path;

	if (fetch_command_at_index(data, &node, index) == EXIT_FAILURE)
		return ;
	if (command_path_is_valid(data, node, &cmd_path) == FALSE)
		return ;
	*pid = fork();
	if (*pid < 0)
	{
		report_error(data, "fork", 1);
		return ;
	}
	if (*pid == 0)
		setup_and_exec_child(data, node, fds, index, cmd_path);
	if (fds[index - 2] != -1)
		close(fds[index - 2]);
	if (fds[index + 1] != -1)
		close(fds[index + 1]);
	free(cmd_path);
	return ;
}
