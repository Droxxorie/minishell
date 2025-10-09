/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_apply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:48:40 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:57:43 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	apply_fd(int fd, int target, t_data *data, int saved[2])
{
	if (fd == -1)
		return (0);
	if (dup2(fd, target) == -1)
	{
		report_error(data, "dup2", -1);
		restore_saved_stdio(saved);
		if (fd > 2)
			close(fd);
		data->exit_status = 1;
		return (-1);
	}
	if (fd > 2)
		close(fd);
	return (0);
}

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
	if (apply_fd(in_fd, STDIN_FILENO, data, saved) == -1)
		return (EXIT_FAILURE);
	if (apply_fd(out_fd, STDOUT_FILENO, data, saved) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	child_setup_io(t_data *data, t_command *node, int *fds, int index)
{
	int	i;
	int	in_fd;
	int	out_fd;
	int	n;

	i = index / 2;
	in_fd = -1;
	out_fd = -1;
	n = compute_n_cmds(data);
	if (open_redirs_for_command(data, node, &in_fd, &out_fd) == -1)
		return (EXIT_FAILURE);
	if (dup_apply_fd(data, in_fd, STDIN_FILENO) == EXIT_FAILURE
		|| dup_apply_fd(data, out_fd, STDOUT_FILENO) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (in_fd == -1 && fds && i > 0 && apply_pipe_in(data, fds, index))
		return (EXIT_FAILURE);
	if (out_fd == -1 && fds && i < n - 1 && apply_pipe_out(data, fds, index))
		return (EXIT_FAILURE);
	if (fds && n > 1)
		close_pipe_fds(fds, 2 * (n - 1));
	return (EXIT_SUCCESS);
}
