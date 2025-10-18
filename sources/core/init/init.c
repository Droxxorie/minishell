/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:35:44 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 19:31:11 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*init_path(char **envp)
{
	int		i;
	char	*path;

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

int	init(t_data *data, char **envp)
{
	int	status;

	if (!data)
		return (EXIT_FAILURE);
	status = init_env(data, envp);
	if (status == EXIT_FAILURE)
	{
		ft_putstr_fd("minishell: Failed to initialize environment\n", 2);
		return (EXIT_FAILURE);
	}
	data->path = init_path(envp);
	data->exit_status = 0;
	data->which_quote = '\0';
	data->commands = NULL;
	data->tokens = NULL;
	data->pipes = NULL;
	data->current_quote = NO_QUOTE;
	data->saw_empty_word_as_command = FALSE;
	data->emit_empty_word = FALSE;
	data->tmp_in_redir_context = FALSE;
	data->tmp_have_content_before = FALSE;
	data->tmp_dollar_index = -1;
	return (EXIT_SUCCESS);
}
