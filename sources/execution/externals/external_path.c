/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:48:05 by eraad             #+#    #+#             */
/*   Updated: 2025/10/03 15:13:17 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static	check_direct_executable(t_data *data, char *command)
{
	char	*full_path;

	full_path = ft_strdup(command);
	if (!full_path)
	{
		report_error(data, "strdup", -1);
		return (NULL);
	}
	if (access(full_path, X_OK | F_OK) == 0)
		return (full_path);
	data->exit_status = 127;
	report_error2(command, "command not found");
	free(full_path);
	return (NULL);
}

static char	*find_executable_in_paths(char **paths, char *command)
{
	int		i;
	char	*full_path;

	if (!paths || !command)
		return (NULL);
	i = 0;
	while (paths && paths[i])
	{
		full_path = ft_strjoin(paths[i], command);
		if (!full_path)
		{
			report_error(NULL, "strjoin", -1);
			free_char_array(paths);
			return (NULL);
		}
		if (access(full_path, X_OK | F_OK) == 0)
		{
			free_char_array(paths);
			return (full_path);
		}
		i++;
	}
	free_char_array(paths);
	return (NULL);
}

static char	*resolve_command_path(t_data *data, t_command *node)
{
	char		*command_path;
	char		**paths;
	struct stat	path_stat;

	if (!node || !node->argv[0] || !data->env_copy)
		return (NULL);
	if (stat(node->argv[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		data->exit_status = 126;
		report_error2(node->argv[0], "is a directory");
		return (NULL);
	}
	paths = build_exec_search_paths(data, data->env_copy);
	command_path = find_executable_in_paths(paths, node->argv[0]);
	if (command_path)
		return (command_path);
	return (check_direct_executable(data, node->argv[0]));
}

t_bool	command_path_is_valid(t_data *data, t_command *node,
		const char **command_path)
{
	*command_path = resolve_command_path(data, node);
	if (!*command_path)
		return (FALSE);
	return (TRUE);
}
