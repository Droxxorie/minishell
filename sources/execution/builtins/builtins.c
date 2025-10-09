/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:53:56 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:48:43 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

pid_t	launch_builtin_child(t_data *data, t_command *node, int *fds, int index)
{
	pid_t	pid;
	int		number_of_commands;

	number_of_commands = data->pipes->nb + 1;
	pid = fork();
	if (pid < 0)
		return (report_error(data, "fork", -1), (pid_t) - 1);
	if (pid == 0)
	{
		setup_child_signals();
		if (child_setup_io(data, node, fds, index) == EXIT_FAILURE)
			exit(1);
		if (dispatch_builtin(data, node) == EXIT_FAILURE)
			exit(1);
		exit(data->exit_status);
	}
	parent_close_after_fork(fds, index, number_of_commands);
	return (pid);
}

int	handle_builtin_command(t_data *data, t_command *node)
{
	int	saved[2];
	int	n;
	int	status;

	n = 1;
	if (data->pipes)
		n = data->pipes->nb + 1;
	if (n != 1)
		return (EXIT_FAILURE);
	if (apply_redirections_in_parent(data, node, saved) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	status = dispatch_builtin(data, node);
	if (restore_saved_stdio(saved) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (status == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

t_bool	is_builtin_command(t_command *node)
{
	if (!node || !node->command)
		return (FALSE);
	if (ft_strcmp(node->command, "cd") == 0 || ft_strcmp(node->command,
			"echo") == 0 || ft_strcmp(node->command, "env") == 0
		|| ft_strcmp(node->command, "exit") == 0 || ft_strcmp(node->command,
			"export") == 0 || ft_strcmp(node->command, "pwd") == 0
		|| ft_strcmp(node->command, "unset") == 0)
		return (TRUE);
	return (FALSE);
}

int	dispatch_builtin(t_data *data, t_command *node)
{
	int	status;

	status = EXIT_SUCCESS;
	if (!node || !node->command)
		return (EXIT_FAILURE);
	if (ft_strcmp(node->command, "cd") == 0)
		status = execute_builtin_cd(data, node);
	else if (ft_strcmp(node->command, "echo") == 0)
		status = execute_builtin_echo(node->argv);
	else if (ft_strcmp(node->command, "env") == 0)
		status = execute_builtin_env(data, node->argv, STDOUT_FILENO);
	else if (ft_strcmp(node->command, "export") == 0)
		status = execute_builtin_export(data, node->argv, STDOUT_FILENO);
	else if (ft_strcmp(node->command, "pwd") == 0)
		status = execute_builtin_pwd(data, node);
	else if (ft_strcmp(node->command, "unset") == 0)
		status = execute_builtin_unset(data, node->argv);
	else if (ft_strcmp(node->command, "exit") == 0)
		status = execute_builtin_exit(data, node->argv);
	else
		return (EXIT_FAILURE);
	data->exit_status = status;
	return (EXIT_SUCCESS);
}
