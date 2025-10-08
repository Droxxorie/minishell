/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:49:18 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 15:49:49 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_heredoc_line(char *line, char *limiter, int write_fd)
{
	if (!line)
		return (EXIT_FAILURE);
	if (ft_strcmp(line, limiter) == 0)
	{
		free(line);
		return (EXIT_FAILURE);
	}
	ft_putstr_fd(line, write_fd);
	ft_putstr_fd("\n", write_fd);
	free(line);
	return (EXIT_SUCCESS);
}

static int	fill_heredoc(int write_fd, char *limiter)
{
	char	*line;

	if (!limiter || !*limiter)
	{
		report_error2("heredoc:", "limiter is missing");
		close(write_fd);
		return (EXIT_FAILURE);
	}
	setup_heredoc_signals();
	while (g_waiting != 3)
	{
		line = readline("heredoc> ");
		if (g_waiting == 3)
		{
			free(line);
			close(write_fd);
			return (EXIT_FAILURE);
		}
		if (process_heredoc_line(line, limiter, write_fd) == EXIT_FAILURE)
			break ;
	}
	close(write_fd);
	return (EXIT_SUCCESS);
}

int	setup_heredoc_node(t_data *data, t_redir *redir)
{
	int	pipes_fd[2];

	if (pipe(pipes_fd) == -1)
	{
		report_error2("heredoc:", "pipe failed");
		return (EXIT_FAILURE);
	}
	if (fill_heredoc(pipes_fd[1], redir->value) == EXIT_FAILURE)
	{
		close(pipes_fd[0]);
		data->exit_status = 130;
		return (EXIT_FAILURE);
	}
	redir->fd = pipes_fd[0];
	return (EXIT_SUCCESS);
}
