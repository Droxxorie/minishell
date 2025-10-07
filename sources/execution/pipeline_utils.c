/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 01:47:20 by eraad             #+#    #+#             */
/*   Updated: 2025/10/07 20:12:37 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	child_dup_fds(t_data *data, int *fds, int index, int number_of_commands)
// {
// 	t_bool	is_first;
// 	t_bool	is_last;

// 	if (number_of_commands <= 1)
// 		return (EXIT_SUCCESS);
// 	if (index == 0)
// 		is_first = TRUE;
// 	else
// 		is_first = FALSE;
// 	if (index == (number_of_commands - 1) * 2)
// 		is_last = TRUE;
// 	else
// 		is_last = FALSE;
// 	if (!is_first)
// 		if (dup2(fds[index - 2], STDIN_FILENO) == -1)
// 			return (report_error(data, "dup2", EXIT_FAILURE), EXIT_FAILURE);
// 	if (!is_last)
// 		if (dup2(fds[index + 1], STDOUT_FILENO) == -1)
// 			return (report_error(data, "dup2", EXIT_FAILURE), EXIT_FAILURE);
// 	return (EXIT_SUCCESS);
// }

int	open_redirs_output(t_data *data, int *out_fd)
{
	int	fd;
	int	flags;

	*out_fd = -1;
	if (data->output.type == REDIR_OUT || data->output.type == REDIR_APPEND)
	{
		if (!data->output.value || data->output.value[0] == '\0')
		{
			// Si tu arrives ici, soit le parseur n’a pas mis de nom de fichier,
			// soit c’est un reste ; dans bash ce serait “ambiguous redirect”.
			report_error(data, "ambiguous redirect", -1);
			return (-1);
		}
		flags = O_WRONLY | O_CREAT | ((data->output.type == REDIR_OUT) ? O_TRUNC : O_APPEND);
		fd = open(data->output.value, flags, 0644);
		if (fd == -1)
		{
			// message exact selon errno
			report_error2("No such file or directory", data->output.value);
			return (-1);
		}
		*out_fd = fd;
	}
	return (0);
}

int	open_redirs_input(t_data *data, int *in_fd)
{
	int	fd;

	*in_fd = -1;
	if (data->input.type == REDIR_IN)
	{
		if (!data->input.value || data->input.value[0] == '\0')
		{
			report_error(data, "ambiguous redirect", -1);
			return (-1);
		}
		fd = open(data->input.value, O_RDONLY);
		if (fd == -1)
		{
			report_error2("No such file or directory: ", data->input.value);
			return (-1);
		}
		*in_fd = fd;
	}
	else if (data->input.type == HEREDOC)
	{
		// selon ton stockage ; sinon adapte
		fd = (data->input.fd >= 0) ? dup(data->input.fd) : -1;
		if (fd == -1)
		{
			report_error(data, "dup", -1);
			return (-1);
		}
		*in_fd = fd;
	}
	return (0);
}

// static int	open_redirs_other(t_data *data, int *out_fd)
// {
// 	int fd;

// 	if (data->output.type == REDIR_OUT)
// 	{
// 		fd = open(data->output.value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (fd == -1)
// 			return (report_error2(data->output.value, "Permission denied"), -1);
// 		*out_fd = fd;
// 	}
// 	else if (data->output.type == REDIR_APPEND)
// 	{
// 		fd = open(data->output.value, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (fd == -1)
// 			return (report_error2(data->output.value, "Permission denied"), -1);
// 		*out_fd = fd;
// 	}
// 	return (0);
// }

// int	open_redirs(t_data *data, int *in_fd, int *out_fd)
// {
// 	int	fd;

// 	*in_fd = -1;
// 	*out_fd = -1;
// 	if (data->input.type == REDIR_IN)
// 	{
// 		fd = open(data->input.value, O_RDONLY);
// 		if (fd == -1)
// 			return (report_error2(data->input.value,
// 					"No such file or directory"), -1);
// 		*in_fd = fd;
// 	}
// 	else if (data->input.type == HEREDOC)
// 	{
// 		if (data->input.fd >= 0)
// 		{
// 			fd = dup(data->input.fd);
// 			if (fd == -1)
// 				return (report_error(data, "dup", -1), -1);
// 			*in_fd = fd;
// 		}
// 	}
// 	return (open_redirs_other(data, out_fd));
// }

int	apply_redirections_in_child(t_data *data)
{
	int	in_fd;
	int	out_fd;

	// if (open_redirs(data, &in_fd, &out_fd) == -1)
	// 	return (EXIT_FAILURE);
	if (open_redirs_input(data, &in_fd) == -1)
		return (EXIT_FAILURE);
	if (open_redirs_output(data, &out_fd) == -1)
		return (EXIT_FAILURE);
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			return (report_error(data, "dup2", EXIT_FAILURE), EXIT_FAILURE);
		if (in_fd > 2)
			close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			return (report_error(data, "dup2", EXIT_FAILURE), EXIT_FAILURE);
		if (out_fd > 2)
			close(out_fd);
	}
	return (EXIT_SUCCESS);
}

static void	update_last_status(t_data *data, int status)
{
	int	sig;

	if (WIFEXITED(status))
	{
		data->exit_status = WEXITSTATUS(status);
		return ;
	}
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
		else if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		data->exit_status = 128 + sig;
	}
}

int	wait_and_cleanup_pipeline(t_data *data, pid_t *pids, int count, int *fds)
{
	int		i;
	int		status;
	int		nb_fds;
	pid_t	wait;

	if (!data || !pids || count <= 0)
		return (EXIT_FAILURE);
	nb_fds = 2 * (count - 1);
	if (nb_fds > 0 && fds)
		close_pipe_fds(fds, nb_fds);
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			wait = waitpid(pids[i], &status, 0);
			while (wait == -1 && errno == EINTR)
				wait = waitpid(pids[i], &status, 0);
			if (wait > 0 && i == count - 1)
				update_last_status(data, status);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
