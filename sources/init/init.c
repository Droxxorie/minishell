/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:53:56 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 20:37:46 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *init_path(char **envp)
{
	int i;
	char *path;

	i = 0;
	path = NULL;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			if (!path)
				return (report_error(NULL, "strdup", -1), NULL);
			return (path);
		}
		i++;
	}
	return (NULL);
}

static void init_io(t_data *data)
{
	data->input.type = REDIR_IN;
	data->input.value = NULL;
	data->input.fd = 0;
	data->input.quote = NO_QUOTE;
	data->output.type = REDIR_OUT;
	data->output.value = NULL;
	data->output.fd = 1;
	data->output.quote = NO_QUOTE;
}

int init(t_data *data, char **envp)
{
	if (init_env(data, envp) == EXIT_FAILURE)
	{
		ft_putstr_fd("minishell: Failed to initialize environment\n", 2);
		return (EXIT_FAILURE);
	}
	data->path = init_path(envp);
	if (!data->path && envp && envp[0] != NULL)
	{
		ft_putstr_fd("minishell: PATH not found\n", 2);
		return (EXIT_FAILURE);
	}
	init_io(data);
	data->exit_status = 0;
	return (EXIT_SUCCESS);
}
