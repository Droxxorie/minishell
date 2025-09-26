/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:08:19 by eraad             #+#    #+#             */
/*   Updated: 2025/09/26 17:55:14 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int setup_redirection(t_redirection *redir, t_token *token, int flags)
{
	redir->type = token->type;
	redir->quote = token->quote;
	if (redir->value)
		free(redir->value);
	redir->value = ft_strdup(token->next->value);
	if (!redir->value)
		return (perror("minishell: strdup"), EXIT_FAILURE);
	if (redir->type != HEREDOC)
	{
		redir->fd = open(redir->value, flags, 0644);
		if (redir->fd == -1)
			return (perror("no such file or directory"), EXIT_FAILURE);

	}
	else
	{
		g_waiting = 2;
		redir->fd = open("heredoc.tmp", flags, 0644);
		if (redir->fd == -1)
			return (perror("minishell: heredoc.tmp"), EXIT_FAILURE);
		setup_heredoc_signals();
	}
	return (EXIT_SUCCESS);
}

int handle_redirection_fd(t_redirection *redir, t_token *token, int flags)
{
	if (redir->fd >= 3)
		close(redir->fd);
	if (!token->next)
	{
		print_syntax_error(token->value[0], 7);
		return (EXIT_FAILURE);
	}
	return (setup_redirection(redir, token, flags));
}

static int process_heredoc_line(char *line, char *limiter, int write_fd)
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

static int fill_heredoc(int write_fd, char *limiter)
{
	char	*line;

	if (!limiter || !*limiter)
	{
		perror("minishell: heredoc limiter is missing");
		close(write_fd);
		return (EXIT_FAILURE);
	}
	setup_heredoc_signals();
	while (g_waiting != 3)
	{
		if (g_waiting == 3)
			return (EXIT_FAILURE);
		line = readline("heredoc> ");
		if (g_waiting == 3)
		{
			if (line)
				free(line);
			close(write_fd);
			return (EXIT_FAILURE);
		}
		if (process_heredoc_line(line, limiter, write_fd) == EXIT_FAILURE)
			break ;
	}
	return (close(write_fd), EXIT_SUCCESS);
}

int setup_heredoc(t_data *data, char *limiter)
{
	int	pipes_fd[2];
	int	write_fd;

	if (pipe(pipes_fd) == -1)
	{
		perror("minishell: pipe");
		return (EXIT_FAILURE);
	}
	data->input.fd = pipes_fd[0];
	write_fd = pipes_fd[1];
	if (fill_heredoc(write_fd, limiter) == EXIT_FAILURE)
	{
		close(write_fd);
		close(data->input.fd);
		data->exit_status = 130;
		return (EXIT_FAILURE);
	}
	close(write_fd);
	return (EXIT_SUCCESS);
}
