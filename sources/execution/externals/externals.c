/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:55:49 by eraad             #+#    #+#             */
/*   Updated: 2025/10/11 12:55:29 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	exec_command(t_data *data, t_command *node, char *command_path)
{
	char	**envp;
	int		err;

	if (!data || !node || !command_path)
		cleanup_and_exit(data, 126);
	envp = env_list_to_array(data->env_copy);
	if (!envp)
		cleanup_and_exit(data, 126);
	execve(command_path, node->argv, envp);
	err = errno;
	free_char_array(envp);
	free(command_path);
	if (err == ENOENT)
	{
		report_error2(node->argv[0], ": command not found");
		cleanup_and_exit(data, 127);
	}
	if (err == EACCES)
	{
		report_error2(node->argv[0], ": Permission denied");
		cleanup_and_exit(data, 126);
	}
	report_error2(node->argv[0], ": execve error");
	cleanup_and_exit(data, 126);
	return (EXIT_FAILURE);
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

static void	child_exec_command(t_data *data, t_command *node, int *fds,
		int index)
{
	char	*cmd_path;

	setup_child_signals();
	if (child_setup_io(data, node, fds, index) == EXIT_FAILURE)
		cleanup_and_exit(data, 1);
	if (!command_path_is_valid(data, node, &cmd_path))
		cleanup_and_exit(data, data->exit_status);
	exec_command(data, node, cmd_path);
	report_error(data, "execve", 1);
	cleanup_and_exit(data, 127);
}

int	handle_external_command(t_data *data, int *fds, int index, pid_t *pid)
{
	t_command	*node;

	if (fetch_command_at_index(data, &node, index) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	*pid = fork();
	if (*pid < 0)
		return (report_error(data, "fork", 1), EXIT_FAILURE);
	if (*pid == 0)
		child_exec_command(data, node, fds, index);
	parent_close_after_fork(fds, index, compute_n_cmds(data));
	return (EXIT_SUCCESS);
}
