/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:40:16 by eraad             #+#    #+#             */
/*   Updated: 2025/10/04 19:26:18 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char *find_env_value(t_data *data, const char *key)
{
	const t_env	*temp;

	if (!data || !data->env_copy || !key)
		return (NULL);
	temp = data->env_copy;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

int	restore_saved_stdio(int *saved_stdio)
{
	int status;

	if (!saved_stdio)
		return (EXIT_FAILURE);
	status = EXIT_SUCCESS;
	if (saved_stdio[0] == -1 || dup2(saved_stdio[0], STDIN_FILENO) == -1)
		status = EXIT_FAILURE;
	if (saved_stdio[1] == -1 || dup2(saved_stdio[1], STDOUT_FILENO) == -1)
		status = EXIT_FAILURE;
	if (saved_stdio[0] != -1)
		close(saved_stdio[0]);
	if (saved_stdio[1] != -1)
		close(saved_stdio[1]);
	saved_stdio[0] = -1;
	saved_stdio[1] = -1;
	if (status == EXIT_FAILURE)
		report_error(NULL, "dup2", -1);
	return (status);
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
		fds[i] = -1;
		i++;
	}
	return ;
}

int	save_stdio(int *saved_stdio)
{
	if (!saved_stdio)
		return (EXIT_FAILURE);
	saved_stdio[0] = -1;
	saved_stdio[1] = -1;
	saved_stdio[0] = dup(STDIN_FILENO);
	if (saved_stdio[0] == -1)
	{
		report_error(NULL, "dup", -1);
		return (EXIT_FAILURE);
	}
	saved_stdio[1] = dup(STDOUT_FILENO);
	if (saved_stdio[1] == -1)
	{
		close(saved_stdio[0]);
		saved_stdio[0] = -1;
		report_error(NULL, "dup", -1);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
