/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:55:49 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:49:03 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	exec_command(t_data *data, t_command *node, char *command_path)
{
	char	**envp;
	int		err;

	if (!data || !node || !command_path)
		exit(126);
	envp = env_list_to_array(data->env_copy);
	if (!envp)
		exit(126);
	execve(command_path, node->argv, envp);
	err = errno;
	free_char_array(envp);
	free(command_path);
	if (err == ENOENT)
	{
		report_error2(node->argv[0], ": command not found");
		exit(127);
	}
	if (err == EACCES)
	{
		report_error2(node->argv[0], ": Permission denied");
		exit(126);
	}
	report_error2(node->argv[0], ": execve error");
	exit(126);
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
	*pid = fork();
	if (*pid < 0)
		return (report_error(data, "fork", 1));
	if (*pid == 0)
	{
		setup_child_signals();
		if (child_setup_io(data, node, fds, index) == EXIT_FAILURE)
			exit(1);
		if (!command_path_is_valid(data, node, &cmd_path))
		{
			cleanup_shell_state(data);
			exit(data->exit_status);
		}
		exec_command(data, node, cmd_path);
		report_error(data, "execve", 1);
		cleanup_shell_state(data);
		exit(127);
	}
	parent_close_after_fork(fds, index, compute_n_cmds(data));
}
