/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_apply_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:57:01 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:58:07 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	dup_apply_fd(t_data *data, int fd, int target)
{
	if (fd == -1)
		return (EXIT_SUCCESS);
	if (dup2(fd, target) == -1)
		return (report_error(data, "dup2", 1), EXIT_FAILURE);
	if (fd > 2)
		close(fd);
	return (EXIT_SUCCESS);
}

int	apply_pipe_in(t_data *data, int *fds, int index)
{
	if (dup2(fds[index - 2], STDIN_FILENO) == -1)
		return (report_error(data, "dup2", 1), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	apply_pipe_out(t_data *data, int *fds, int index)
{
	if (dup2(fds[index + 1], STDOUT_FILENO) == -1)
		return (report_error(data, "dup2", 1), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
