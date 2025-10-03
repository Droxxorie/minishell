/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:38:03 by eraad             #+#    #+#             */
/*   Updated: 2025/10/02 12:35:30 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static void	export_add_key(t_data *data, const char *key)
{
	char	*dup_key;

	if (!data || !key || key[0] == '\0')
		return ;
	dup_key = ft_strdup(key);
	if (!dup_key)
	{
		report_error(data, "strdup", -1);
		return ;
	}
	if (add_var(data, &data->export, dup_key, NULL) == EXIT_FAILURE)
	{
		free(dup_key);
		return ;
	}
}

void	handle_export_arg(t_data *data, char *key, char *arg, int key_index)
{
	const char	*offset;

	if (!data || !arg || !key)
		return ;
	offset = ft_strchr(arg, '=');
	if (key_index != -1 && offset)
		env_update_value(data, arg, key_index);
	else if (key_index == -1 && offset)
		env_add_from_arg(data, data->export, key, arg);
	else if (key_index == -1 && !offset)
		export_add_key(data, ft_strdup(key));
	data->export = sort_export_list(data->export, ft_strcmp);
	free(key);
}

void	env_add_from_arg(t_data *data, t_env *env, char *key, char *arg)
{
	const char	*offset;
	char		*value;
	char 		*dup_key;

	if (!data || !arg || !key)
		return ;
	offset = ft_strchr(arg, '=');
	if (!offset)
		return ;
	value = ft_strdup(offset + 1);
	if (!value)
		return (report_error(data, "strdup", -1));
	dup_key = ft_strdup(key);
	if (!dup_key)
	{
		free(value);
		return (report_error(data, "strdup", -1));
	}
	add_var(data, env, dup_key, value);
}

void	env_update_value(t_data *data, const char *arg, int key_index)
{
	t_env		*temp;
	char		*new_value;
	const char	*offset;

	if (!data || !arg || key_index < 0)
		return ;
	offset = ft_strchr(arg, '=');
	if (!offset)
		return ;
	temp = data->env_copy;
	while (temp && key_index > 0)
	{
		temp = temp->next;
		key_index--;
	}
	if (!temp)
		return ;
	new_value = ft_strdup(offset + 1);
	if (!new_value)
		return (report_error(data, "strdup", -1));
	free(temp->value);
	temp->value = new_value;
	return ;
}

int	env_index_of_key(t_env *env, char *key)
{
	t_env	*temp;
	size_t	key_len;
	int		i;

	if (!env || !key || key[0] == '\0')
		return (-1);
	temp = env;
	key_len = ft_strlen(key);
	i = 0;
	while (temp)
	{
		if (ft_strlen(temp->key) == key_len && ft_strncmp(temp->key, key,
				key_len) == 0)
			return (i);
		temp = temp->next;
		i++;
	}
	return (-1);
}
