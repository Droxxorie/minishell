/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:36:19 by eraad             #+#    #+#             */
/*   Updated: 2025/10/02 00:03:15 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	add_shlvl(t_data *data)
{
	t_env	*temp;
	int		shlvl;
	int		i;

	shlvl = 0;
	i = -1;
	temp = data->env_copy;
	while (data->env[++i])
		if (!ft_strncmp(data->env[i], "SHLVL=", 6))
			shlvl = ft_atoll(data->env[i] + 6);
	while (temp)
	{
		if (!ft_strncmp(temp->key, "SHLVL", 5))
		{
			free(temp->value);
			temp->value = ft_itoa(shlvl + 1);
			if (!temp->value)
				return (EXIT_FAILURE);
			return (EXIT_SUCCESS);
		}
		temp = temp->next;
	}
	return (EXIT_FAILURE);
}

static int	add_default_vars(t_data *data, t_env **env_copy)
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (report_error(data, "getcwd", -1), EXIT_FAILURE);
	if (add_var(data, env_copy, ft_strdup("PWD"), pwd) == EXIT_FAILURE)
	{
		free(pwd);
		return (EXIT_FAILURE);
	}
	if (add_var(data, env_copy, ft_strdup("SHLVL"), ft_strdup("1")) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (add_var(data, env_copy, ft_strdup("_"), ft_strdup("/usr/bin/env")) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	fill_env_copy(t_data *data, t_env **env_copy)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 0;
	while (data->env[i])
	{
		j = 0;
		while (data->env[i][j] != '=' && data->env[i][j] != '\0')
			j++;
		key = ft_substr(data->env[i], 0, j);
		value = ft_substr(data->env[i], j + 1, ft_strlen(data->env[i]));
		if (!key || !value)
			return (EXIT_FAILURE);
		if (add_var(data, env_copy, key, value) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_env(t_data *data, char **envp)
{
	t_env	*env_copy;

	env_copy = NULL;
	data->env = envp;
	if (!envp || !envp[0])
	{
		if (add_default_vars(data, &env_copy) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (fill_env_copy(data, &env_copy) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (add_shlvl(data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
