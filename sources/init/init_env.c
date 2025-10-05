/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:36:19 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 20:52:38 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	append_env_from_entry(t_data *data, t_env **env_copy,
		const char *entry)
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

// static int	add_shlvl(t_data *data)
// {
// 	t_env	*temp;
// 	long	shlvl;
// 	int		i;

// 	shlvl = 0;
// 	i = 0;
// 	temp = data->env_copy;
// 	while (data->env[++i])
// 		if (!ft_strncmp(data->env[i], "SHLVL=", 6))
// 			shlvl = ft_atoll(data->env[i] + 6);
// 	while (temp)
// 	{
// 		if (!ft_strncmp(temp->key, "SHLVL", 5))
// 		{
// 			free(temp->value);
// 			temp->value = ft_itoa(shlvl + 1);
// 			if (!temp->value)
// 				return (EXIT_FAILURE);
// 			return (EXIT_SUCCESS);
// 		}
// 		temp = temp->next;
// 	}
// 	return (EXIT_FAILURE);
// }

static int	add_shlvl(t_data *data)
{
	int		i;
	long	shlvl;

	i = 0;
	shlvl = 0;
	if (data->env)
		while (data->env[i])
		{
			if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
			{
				shlvl = ft_atoll(data->env[i] + 6);
				break ;
			}
			i++;
		}
	if (update_shlvl(data, shlvl) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	if (create_shlvl(data, shlvl) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

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
