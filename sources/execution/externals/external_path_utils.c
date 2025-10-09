/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_path_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:49:44 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:23:43 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*normalize_path_dir(const char *dir)
{
	size_t	len;
	char	*out;

	if (!dir)
		return (NULL);
	len = ft_strlen(dir);
	if (len == 0)
	{
		out = ft_strdup("./");
		if (!out)
			report_error(NULL, "strdup", -1);
		return (out);
	}
	if (dir[len - 1] == '/')
	{
		out = ft_strdup(dir);
		if (!out)
			report_error(NULL, "strdup", -1);
		return (out);
	}
	out = ft_strjoin(dir, "/");
	if (!out)
		report_error(NULL, "strjoin", -1);
	return (out);
}

static char	**normalize_path_split(t_data *data, const char **path_split)
{
	int		count;
	char	**out;
	int		i;

	if (!path_split)
		return (NULL);
	count = 0;
	while (path_split[count])
		count++;
	out = (char **)malloc(sizeof(char *) * (count + 1));
	if (!out)
		return (report_error(data, "malloc", -1), NULL);
	i = 0;
	while (i < count)
	{
		out[i] = normalize_path_dir((const char *)path_split[i]);
		if (!out[i])
		{
			free_char_array(out);
			return (report_error(data, "strjoin", -1), NULL);
		}
		i++;
	}
	out[count] = NULL;
	return (out);
}

char	**build_exec_search_paths(t_data *data, const t_env *env)
{
	char	**path_split;
	char	**paths;

	while (env && ft_strcmp(env->key, "PATH") != 0)
		env = env->next;
	if (!env || !env->value)
		return (NULL);
	path_split = ft_split(env->value, ':');
	if (!path_split)
		return (report_error(data, "split", -1), NULL);
	paths = normalize_path_split(data, (const char **)path_split);
	free_char_array(path_split);
	return (paths);
}
