/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:50:26 by eraad             #+#    #+#             */
/*   Updated: 2025/10/11 16:46:29 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static void	dup_close_and_free(t_data *data, int fd1, int fd2)
// {
// 	if (fd1 != -1)
// 		dup2(fd1, fd2);
// 	if (fd1 > 2)
// 		close(fd1);
// 	free_pipes_all(data);
// }

static int	count_commands(t_command *commands)
{
	int			count;
	t_command	*current;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

// int	executor(t_data *data)
// {
// 	int	number_of_commands;
// 	int	saved_stdin;
// 	int	devnull;

// 	number_of_commands = count_commands(data->commands);
// 	if (number_of_commands <= 0)
// 		return (EXIT_FAILURE);
// 	if (prepare_pipes(data, number_of_commands) == EXIT_FAILURE)
// 		return (EXIT_FAILURE);
// 	saved_stdin = dup(STDIN_FILENO);
// 	if (saved_stdin != -1)
// 	{
// 		devnull = open("/dev/null", O_RDONLY);
// 		if (devnull != -1)
// 		{
// 			dup2(devnull, STDIN_FILENO);
// 			if (devnull > 2)
// 				close(devnull);
// 		}
// 	}
// 	if (launch_pipeline(data) == EXIT_FAILURE)
// 		return (dup_close_and_free(data, saved_stdin, STDIN_FILENO),
// 			EXIT_FAILURE);
// 	dup_close_and_free(data, saved_stdin, STDIN_FILENO);
// 	return (EXIT_SUCCESS);
// }

int	executor(t_data *data)
{
	int	number_of_commands;

	number_of_commands = count_commands(data->commands);
	if (number_of_commands <= 0)
		return (EXIT_FAILURE);
	if (prepare_pipes(data, number_of_commands) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (launch_pipeline(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free_pipes_all(data);
	return (EXIT_SUCCESS);
}
