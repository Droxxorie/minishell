/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:36:42 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 16:20:24 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	add_default_vars(t_data *data, t_env **env_copy)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (report_error(data, "getcwd", -1), EXIT_FAILURE);
	if (add_var(data, env_copy, ft_strdup("PWD"), pwd) == EXIT_FAILURE)
	{
		free(pwd);
		return (EXIT_FAILURE);
	}
	if (add_var(data, env_copy, ft_strdup("SHLVL"),
			ft_strdup("1")) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (add_var(data, env_copy, ft_strdup("_"),
			ft_strdup("/usr/bin/env")) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	append_env_from_entry(t_data *data, t_env **env_copy, const char *entry)
{
	char	*key;
	char	*value;
	size_t	length;

	key = NULL;
	value = NULL;
	length = ft_strlen(entry);
	if (extract_key_value(entry, &key, &value, length) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (add_var(data, env_copy, key, value) == EXIT_FAILURE)
	{
		free(key);
		if (value)
			free(value);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	fill_env_copy(t_data *data, t_env **env_copy)
{
	int	i;

	if (!data->env)
		return (EXIT_FAILURE);
	i = 0;
	while (data->env[i])
	{
		if (append_env_from_entry(data, env_copy, data->env[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_env(t_data *data, char **envp)
{
	data->env = envp;
	data->env_copy = NULL;
	if (!envp || !envp[0])
	{
		if (add_default_vars(data, &data->env_copy) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (fill_env_copy(data, &data->env_copy) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (add_shlvl(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
