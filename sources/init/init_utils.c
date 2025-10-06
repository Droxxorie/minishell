/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:44:48 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 15:43:55 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	update_shlvl(t_data *data, int shlvl)
{
	t_env	*temp;
	char	*new_value;

	temp = data->env_copy;
	while (temp)
	{
		if (strncmp(temp->key, "SHLVL", 5) == 0 && temp->key[5] == '\0')
		{
			new_value = ft_itoa((int)(shlvl + 1));
			if (!new_value)
				return (EXIT_FAILURE);
			free(temp->value);
			temp->value = ft_strdup(new_value);
			free(new_value);
			if (!temp->value)
				return (EXIT_FAILURE);
			temp->equal = '=';
			return (EXIT_SUCCESS);
		}
		temp = temp->next;
	}
	return (EXIT_FAILURE);
}

int	create_shlvl(t_data *data, int shlvl)
{
	char	*key;
	char	*value;
	int		status;

	key = ft_strdup("SHLVL");
	value = ft_itoa((int)(shlvl + 1));
	if (!key || !value)
	{
		free(key);
		free(value);
		return (EXIT_FAILURE);
	}
	status = add_var(data, &data->env_copy, key, value);
	if (status == EXIT_FAILURE)
	{
		free(key);
		free(value);
	}
	return (status);
}

int	extract_key_value(const char *entry, char **key, char **value, int length)
{
	int	j;

	j = 0;
	while (entry[j] && entry[j] != '=')
		j++;
	*key = ft_substr(entry, 0, j);
	if (!*key)
		return (EXIT_FAILURE);
	if (entry[j] == '=')
	{
		if (length > j + 1)
			*value = ft_substr(entry, j + 1, length - (j + 1));
		else
			*value = ft_substr(entry, j + 1, 0);
		if (!*value)
		{
			free(*key);
			*key = NULL;
			return (EXIT_FAILURE);
		}
	}
	else
		*value = NULL;
	return (EXIT_SUCCESS);
}

t_env	*env_last_var(t_env *env)
{
	if (!env)
		return (NULL);
	while (env->next)
		env = env->next;
	return (env);
}

int	add_var(t_data *data, t_env **env, char *key, char *value)
{
	t_env	*new_var;
	t_env	*temp;

	if (!key)
		return (EXIT_FAILURE);
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (report_error(data, "malloc", -1), EXIT_FAILURE);
	new_var->key = key;
	if (value)
		new_var->equal = '=';
	else
		new_var->equal = '\0';
	new_var->value = value;
	new_var->next = NULL;
	if (!*env)
	{
		*env = new_var;
		if (data && env == &data->env_copy)
			data->env_copy = *env;
		return (EXIT_SUCCESS);
	}
	temp = env_last_var(*env);
	temp->next = new_var;
	return (EXIT_SUCCESS);
}
