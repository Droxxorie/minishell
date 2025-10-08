/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_apply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:48:40 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 15:49:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	apply_redirections_in_parent(t_data *data, t_command *node, int saved[2])
{
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	if (save_stdio(saved) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (open_redirs_for_command(data, node, &in_fd, &out_fd) == -1)
	{
		restore_saved_stdio(saved);
		data->exit_status = 1;
		return (EXIT_FAILURE);
	}
	if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
	{
		report_error(data, "dup2", -1);
		restore_saved_stdio(saved);
		if (in_fd > 2)
			close(in_fd);
		data->exit_status = 1;
		return (EXIT_FAILURE);
	}
	if (in_fd > 2)
		close(in_fd);
	if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
	{
		report_error(data, "dup2", -1);
		restore_saved_stdio(saved);
		if (out_fd > 2)
			close(out_fd);
		data->exit_status = 1;
		return (EXIT_FAILURE);
	}
	if (out_fd > 2)
		close(out_fd);
	return (EXIT_SUCCESS);
}

int	child_setup_io(t_data *data, t_command *node, int *fds, int index, int n)
{
	int	i;
	int	in_fd;
	int	out_fd;

	i = index / 2;
	in_fd = -1;
	out_fd = -1;
	if (open_redirs_for_command(data, node, &in_fd, &out_fd) == -1)
		return (EXIT_FAILURE);
	if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
		return (report_error(data, "dup2", 1), EXIT_FAILURE);
	if (in_fd > 2)
		close(in_fd);
	if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
		return (report_error(data, "dup2", 1), EXIT_FAILURE);
	if (out_fd > 2)
		close(out_fd);
	if (in_fd == -1 && fds && i > 0 && dup2(fds[index - 2], STDIN_FILENO) == -1)
		return (report_error(data, "dup2", 1), EXIT_FAILURE);
	if (out_fd == -1 && fds && i < n - 1 && dup2(fds[index + 1],
			STDOUT_FILENO) == -1)
		return (report_error(data, "dup2", 1), EXIT_FAILURE);
	if (fds && n > 1)
		close_pipe_fds(fds, 2 * (n - 1));
	return (EXIT_SUCCESS);
}
