/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:02:05 by eraad             #+#    #+#             */
/*   Updated: 2025/10/04 19:20:27 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	dispatch_builtin(t_data *data, t_command *node)
{
	int	status;

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
	return (status);
}

static int	setup_redirections(t_data *data, int *fds, int index,
		int *saved_stdio)
{
	if (save_stdio(saved_stdio) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->input.type == HEREDOC)
	{
		if (dup2(data->input.fd, STDIN_FILENO) == -1)
			return (close(saved_stdio[0]), close(saved_stdio[1]),
				report_error(data, "dup2", -1), EXIT_FAILURE);
		close(data->input.fd);
	}
	else if (index / 2 > 0 && fds)
	{
		if (index >= 2 && dup2(fds[index - 2], STDIN_FILENO) == -1)
			return (close(saved_stdio[0]), close(saved_stdio[1]),
				report_error(data, "dup2", -1), EXIT_FAILURE);
	}
	if (index / 2 < data->pipes->nb + 1 && fds)
	{
		if (fds && dup2(fds[index + 1], STDOUT_FILENO) == -1)
			return (close(saved_stdio[0]), close(saved_stdio[1]),
				report_error(data, "dup2", -1), EXIT_FAILURE);
	}
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

int	handle_builtin_command(t_data *data, int *fds, int index, t_command *node)
{
	int	builtin_status;
	int	saved_stdio[2];
	int	N;
	int	i;

	if (data->pipes)
		N = data->pipes->nb + 1;
	else
		N = 1;
	i = index / 2;
	if (setup_redirections(data, fds, index, saved_stdio) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	builtin_status = dispatch_builtin(data, node);
	if (i > 0 && fds)
		safe_close_fd(&fds[index - 2]);
	if (i < N - 1 && fds)
		safe_close_fd(&fds[index + 1]);
	// close_pipe_fds(fds, index); //TODO a mettre dans l'enfant
	if (restore_saved_stdio(saved_stdio) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->exit_status = builtin_status;
	return (builtin_status); //? EXIT_SUCCESS?
}
