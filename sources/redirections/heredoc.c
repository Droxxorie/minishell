/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:49:18 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 13:47:55 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	write_and_free_line(int fd, char *s)
{
	size_t	len;

	if (!s)
		return (EXIT_FAILURE);
	len = ft_strlen(s);
	if (len && write(fd, s, len) == -1)
		return (free(s), EXIT_FAILURE);
	if (write(fd, "\n", 1) == -1)
		return (free(s), EXIT_FAILURE);
	free(s);
	return (EXIT_SUCCESS);
}

static int	process_heredoc_line(t_data *data, char **line, t_redir *redir,
		int *pipefd)
{
	char	*to_write;

	*line = readline("> ");
	if (!*line || g_waiting == 3 || (redir->value && ft_strcmp(*line,
				redir->value) == 0))
		return (2);
	if (redir->quote == NO_QUOTE)
		to_write = expand_heredoc_line(data, *line);
	else
		to_write = ft_strdup(*line);
	free(*line);
	if (!to_write)
		return (close(pipefd[0]), close(pipefd[1]), report_error(data,
				"heredoc alloc", -1), EXIT_FAILURE);
	if (write_and_free_line(pipefd[1], to_write) == EXIT_FAILURE)
		return (close(pipefd[0]), close(pipefd[1]), report_error(data,
				"heredoc write", -1), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	setup_heredoc_node(t_data *data, t_redir *redir)
{
	int		pipefd[2];
	char	*line;
	int		status;

	line = NULL;
	pipefd[0] = -1;
	pipefd[1] = -1;
	status = 0;
	if (pipe(pipefd) == -1)
		return (report_error(data, "pipe", -1), EXIT_FAILURE);
	while (1)
	{
		status = process_heredoc_line(data, &line, redir, pipefd);
		if (status == 2)
			break ;
		if (status == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (line)
		free(line);
	close(pipefd[1]);
	if (g_waiting == 3)
		return (close(pipefd[0]), data->exit_status = 130, EXIT_FAILURE);
	redir->fd = pipefd[0];
	return (EXIT_SUCCESS);
}
