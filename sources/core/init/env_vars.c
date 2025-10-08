/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:37:56 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 23:11:22 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	add_shlvl(t_data *data)
{
	int		i;
	long	shlvl;

	i = 0;
	shlvl = 0;
	if (data->env)
	{
		while (data->env[i])
		{
			if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
			{
				shlvl = ft_atoll(data->env[i] + 6);
				break ;
			}
			i++;
		}
	}
	if (update_shlvl(data, shlvl) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	if (create_shlvl(data, shlvl) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
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
