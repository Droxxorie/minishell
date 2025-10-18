/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:48:05 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 11:49:13 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*check_direct_executable(t_data *data, char *cmd)
{
	char	*full_path;

	full_path = ft_strdup(cmd);
	if (!full_path)
	{
		report_error(data, "strdup", -1);
		return (NULL);
	}
	if (access(full_path, F_OK) == -1)
	{
		if (ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "/", 1) == 0)
			report_error2(cmd, ": No such file or directory");
		else
			report_error2(cmd, ": command not found");
		data->exit_status = 127;
		return (free(full_path), NULL);
	}
	if (access(full_path, X_OK) == -1)
	{
		report_error2(cmd, ": Permission denied");
		data->exit_status = 126;
		return (free(full_path), NULL);
	}
	return (full_path);
}

static char	*join_dir_command(const char *dir, const char *command)
{
	char	*temp;
	char	*full_path;

	if (!dir || !command)
		return (NULL);
	temp = ft_strdup(dir);
	if (!temp)
	{
		report_error(NULL, "strdup", -1);
		return (NULL);
	}
	full_path = ft_strjoin(temp, command);
	free(temp);
	if (!full_path)
		report_error(NULL, "strjoin", -1);
	return (full_path);
}

static char	*find_executable_in_paths(char **paths, char *command)
{
	int		i;
	char	*full_path;

	if (!paths || !command)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = join_dir_command(paths[i], command);
		if (!full_path)
		{
			free_char_array(paths);
			return (NULL);
		}
		if (access(full_path, X_OK | F_OK) == 0)
		{
			free_char_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_char_array(paths);
	return (NULL);
}

static char	*resolve_command_path(t_data *data, t_command *n)
{
	char		*command_path;
	struct stat	path_stat;

	if (!n || !data->env_copy)
		return (NULL);
	if (!n->argv[0])
	{
		safe_putstr_fd("minishell: command not found\n", 2);
		data->exit_status = 127;
		return (NULL);
	}
	if (stat(n->argv[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		if (ft_strncmp(n->argv[0], "./", 2) == 0 || ft_strncmp(n->argv[0], "/",
				1) == 0)
		{
			report_error2(n->argv[0], ": Is a directory");
			data->exit_status = 126;
		}
		else
		{
			report_error2(n->argv[0], ": command not found");
			data->exit_status = 127;
		}
		return (NULL);
	}
	command_path = find_executable_in_paths(build_exec_search_paths(data,
				data->env_copy), n->argv[0]);
	if (command_path)
		return (command_path);
	return (check_direct_executable(data, n->argv[0]));
}

t_bool	command_path_is_valid(t_data *data, t_command *node,
		char **command_path)
{
	if (!node || !node->command || node->command[0] == '\0')
	{
		/* comme bash : message générique et code 127 */
		safe_putstr_fd("minishell: command not found\n", 2);
		data->exit_status = 127;
		return (FALSE);
			/* IMPORTANT: ne pas considérer cela comme "fatal" pour tout le pipeline */
	}
	*command_path = resolve_command_path(data, node);
	if (!*command_path)
		return (FALSE);
	return (TRUE);
}
