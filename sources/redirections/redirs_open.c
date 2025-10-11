/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_open.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:47:08 by eraad             #+#    #+#             */
/*   Updated: 2025/10/11 11:48:36 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_heredoc(t_data *data, t_redir *redir, int *in_fd)
{
	int	fd;

	fd = -1;
	if (redir->fd >= 0)
		fd = dup(redir->fd);
	if (fd == -1)
		return (report_error(data, "dup", -1), EXIT_FAILURE);
	if (*in_fd != -1 && *in_fd > 2)
		close(*in_fd);
	*in_fd = fd;
	return (EXIT_SUCCESS);
}

static int	open_out_append(t_data *data, t_redir *redir, int *out_fd)
{
	int	fd;

	if (!redir->value || redir->value[0] == '\0')
		return (report_error(data, "ambiguous redirect", -1), EXIT_FAILURE);
	if (check_redir_operator(data, redir) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	fd = open(redir->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (report_error3(redir->value, ": ", strerror(errno)),
			EXIT_FAILURE);
	if (*out_fd != -1 && *out_fd > 2)
		close(*out_fd);
	*out_fd = fd;
	return (EXIT_SUCCESS);
}

static int	open_out_trunc(t_data *data, t_redir *redir, int *out_fd)
{
	int	fd;

	if (!redir->value || redir->value[0] == '\0')
		return (report_error(data, "ambiguous redirect", -1), EXIT_FAILURE);
	if (check_redir_operator(data, redir) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	fd = open(redir->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (report_error3(redir->value, ": ", strerror(errno)),
			EXIT_FAILURE);
	if (*out_fd != -1 && *out_fd > 2)
		close(*out_fd);
	*out_fd = fd;
	return (EXIT_SUCCESS);
}

static int	open_in(t_data *data, t_redir *redir, int *in_fd)
{
	int	fd;

	if (!redir->value || redir->value[0] == '\0')
		return (report_error(data, "ambiguous redirect", -1), EXIT_FAILURE);
	if (check_redir_operator(data, redir) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	fd = open(redir->value, O_RDONLY);
	if (fd == -1)
		return (report_error3(redir->value, ": ", strerror(errno)),
			EXIT_FAILURE);
	if (*in_fd != -1 && *in_fd > 2)
		close(*in_fd);
	*in_fd = fd;
	return (EXIT_SUCCESS);
}

int	open_redirs_for_command(t_data *data, t_command *node, int *in_fd,
		int *out_fd)
{
	t_redir	*temp;
	int		status;

	*in_fd = -1;
	*out_fd = -1;
	temp = node->redirs;
	while (temp)
	{
		status = EXIT_SUCCESS;
		if (temp->type == REDIR_IN)
			status = open_in(data, temp, in_fd);
		else if (temp->type == REDIR_OUT)
			status = open_out_trunc(data, temp, out_fd);
		else if (temp->type == REDIR_APPEND)
			status = open_out_append(data, temp, out_fd);
		else if (temp->type == HEREDOC)
			status = open_heredoc(data, temp, in_fd);
		if (status == EXIT_FAILURE)
		{
			// data->exit_status = 2;
			return (-1);
		}
		temp = temp->next;
	}
	return (0);
}
