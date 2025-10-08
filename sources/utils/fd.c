/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:09:50 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 16:10:49 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	restore_saved_stdio(int *saved_stdio)
{
	int	status;

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
