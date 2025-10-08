/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:48:05 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 20:01:02 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*check_direct_executable(t_data *data, char *command)
{
	char	*full_path;

	full_path = ft_strdup(command);
	if (!full_path)
	{
		report_error(data, "strdup", -1);
		return (NULL);
	}
	if (access(full_path, F_OK) == -1)
	{
		if (ft_strncmp(command, "./", 2) == 0 || ft_strncmp(command, "/",
				1) == 0)
			report_error2(command, ": No such file or directory");
		else
			report_error2(command, ": command not found");
		data->exit_status = 127;
		free(full_path);
		return (NULL);
	}
	if (access(full_path, X_OK) == -1)
	{
		report_error2(command, ": Permission denied");
		data->exit_status = 126;
		free(full_path);
		return (NULL);
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

static char	*resolve_command_path(t_data *data, t_command *node)
{
	char		*command_path;
	char		**paths;
	struct stat	path_stat;

	if (!node || !node->argv[0] || !data->env_copy)
		return (NULL);
	if (stat(node->argv[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		if (ft_strncmp(node->argv[0], "./", 2) == 0 || ft_strncmp(node->argv[0],
				"/", 1) == 0)
		{
			report_error2(node->argv[0], ": Is a directory");
			data->exit_status = 126;
		}
		else
		{
			report_error2(node->argv[0], ": command not found");
			data->exit_status = 127;
		}
		return (NULL);
	}
	paths = build_exec_search_paths(data, data->env_copy);
	command_path = find_executable_in_paths(paths, node->argv[0]);
	if (command_path)
		return (command_path);
	return (check_direct_executable(data, node->argv[0]));
}

// static t_bool	resolve_in_path_helper(t_data *data, char **paths,
// 		const char *cmd, int access_result)
// {
// 	if (paths)
// 		free_char_array(paths);
// 	if (access_result)
// 	{
// 		report_error2((char *)cmd, "Permission denied");
// 		data->exit_status = 126;
// 	}
// 	else
// 	{
// 		report_error2((char *)cmd, "command not found");
// 		data->exit_status = 127;
// 	}
// 	return (FALSE);
// }

// static t_bool	resolve_in_path(t_data *data, const char *cmd, char **out)
// {
// 	char	**paths;
// 	int		i;
// 	int		access_result;
// 	char	*full_path;

// 	*out = NULL;
// 	paths = build_exec_search_paths(data, data->env_copy);
// 	i = 0;
// 	access_result = 0;
// 	while (paths && paths[i])
// 	{
// 		full_path = path_join(paths[i], cmd);
// 		if (!full_path)
// 			return (free_char_array(paths), report_error(data, "malloc", 1),
// 				FALSE);
// 		if (access(full_path, X_OK) == 0 && !path_is_dir(full_path))
// 			return (free_char_array(paths), *out = full_path, TRUE);
// 		if (access(full_path, F_OK) == 0)
// 			access_result = 1;
// 		free(full_path);
// 		i++;
// 	}
// 	return (resolve_in_path_helper(data, paths, cmd, access_result));
// }

// static t_bool	resolve_with_slash(t_data *data, const char *cmd,
// char **out)
// {
// 	if (path_is_dir(cmd))
// 	{
// 		report_error2((char *)cmd, " is a directory");
// 		data->exit_status = 126;
// 		return (FALSE);
// 	}
// 	if (access(cmd, F_OK) != 0)
// 	{
// 		report_error2((char *)cmd, strerror(errno));
// 		data->exit_status = 127;
// 		return (FALSE);
// 	}
// 	if (access(cmd, X_OK) != 0)
// 	{
// 		report_error2((char *)cmd, "Permission denied");
// 		data->exit_status = 126;
// 		return (FALSE);
// 	}
// 	*out = ft_strdup(cmd);
// 	if (!*out)
// 	{
// 		report_error(data, "strdup", 1);
// 		return (FALSE);
// 	}
// 	return (TRUE);
// }

// char	*path_join(const char *dir, const char *cmd)
// {
// 	size_t	dir_len;
// 	size_t	cmd_len;
// 	char	*full_path;
// 	size_t	i;
// 	size_t	j;

// 	if (!dir || !cmd)
// 		return (NULL);
// 	dir_len = ft_strlen(dir);
// 	cmd_len = ft_strlen(cmd);
// 	full_path = (char *)malloc(dir_len + cmd_len + 2);
// 	if (!full_path)
// 		return (report_error(NULL, "malloc", -1), NULL);
// 	i = -1;
// 	while (i++ < dir_len)
// 		full_path[i] = dir[i];
// 	if (i > 0 && full_path[i - 1] != '/')
// 		full_path[i++] = '/';
// 	j = 0;
// 	while (j < cmd_len)
// 		full_path[i++] = cmd[j++];
// 	full_path[i] = '\0';
// 	return (full_path);
// }

t_bool	command_path_is_valid(t_data *data, t_command *node,
		char **command_path)
{
	*command_path = resolve_command_path(data, node);
	if (!*command_path)
		return (FALSE);
	return (TRUE);
}

// t_bool	command_path_is_valid(t_data *data, t_command *node, char **path)
// {
// 	const char	*cmd;

// 	if (!data || !node || !node->command || !path)
// 		return (FALSE);
// 	*path = NULL;
// 	cmd = node->command;
// 	if (has_slash(cmd))
// 		return (resolve_with_slash(data, cmd, path));
// 	return (resolve_in_path(data, cmd, path));
// }
