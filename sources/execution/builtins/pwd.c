/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:58:34 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 15:58:58 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_bool	is_valid_pwd_args(t_data *data, t_command *node)
{
	if (node->flags != NULL)
	{
		report_error(data, "pwd: usage: pwd\n", -1);
		return (FALSE);
	}
	return (TRUE);
}

int	execute_builtin_pwd(t_data *data, t_command *node)
{
	char	*cwd;

	if (is_valid_pwd_args(data, node) == FALSE)
		return (EXIT_FAILURE);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		report_error(data, "pwd: error retrieving current directory", -1);
		return (EXIT_FAILURE);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	free(cwd);
	return (EXIT_SUCCESS);
}
