/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:11:01 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 22:56:31 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_fds_from(int start_fd)
{
	int	fd;
	int	max_fd;

	max_fd = 1024;
	fd = start_fd;
	while (fd < max_fd)
	{
		close(fd);
		fd++;
	}
}

void	close_pipe_fds(int *fds, int count)
{
	int	i;

	if (!fds || count <= 0)
		return ;
	i = 0;
	while (i < count)
	{
		if (fds[i] > 2)
			close(fds[i]);
		i++;
	}
	return ;
}

int	safe_close_fd(int *fd)
{
	if (!fd || *fd < 0)
		return (EXIT_SUCCESS);
	if (*fd <= 2)
		return (EXIT_SUCCESS);
	if (close(*fd) == -1)
	{
		report_error(NULL, "close", -1);
		return (EXIT_FAILURE);
	}
	*fd = -1;
	return (EXIT_SUCCESS);
}
