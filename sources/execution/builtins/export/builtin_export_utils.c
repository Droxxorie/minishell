/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:25:30 by eraad             #+#    #+#             */
/*   Updated: 2025/10/02 12:51:49 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

//? A voir pour le arg[0] == '=' case
char	*extract_env_key(char *arg)
{
	char	*key;
	char	*equal_sign;

	// if (arg[0] == '=')
	// {
	// 	key = ft_strdup(arg);
	// 	if (!key)
	// 		return (report_error(NULL, "strdup", -1), NULL);
	// 	return (key);
	// }
	if (!arg || arg[0] == '\0' || arg[0] == '=')
		return (NULL);
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		key = ft_strdup(arg);
		if (!key)
			return (report_error(NULL, "strdup", -1), NULL);
	}
	else
	{
		key = ft_substr(arg, 0, equal_sign - arg);
		if (!key)
			return (report_error(NULL, "substr", -1), NULL);
	}
	return (key);
}

// static t_env	*sort_export_list(t_env *export, int (*cmp)(const char *,
// 			const char *))
// {
// 	t_env	*sorted;
// 	char	*temp_key;
// 	char	*temp_value;

// 	sorted = export;
// 	while (sorted)
// 	{
// 		if (cmp(export->key, export->next->key) > 0)
// 		{
// 			temp_key = export->key;
// 			temp_value = export->value;
// 			export->key = export->next->key;
// 			export->value = export->next->value;
// 			export->next->key = temp_key;
// 			export->next->value = temp_value;
// 			export = sorted;
// 		}
// 		else
// 			sorted = sorted->next;
// 	}
// 	return (sorted);
// }

t_env	*sort_export_list(t_env *head, int (*cmp)(const char *,
			const char *))
{
	t_env	*end;
	t_env	*current;
	t_bool	swapped;

	if (!head)
		return (NULL);
	end = NULL;
	current = head;
	swapped = TRUE;
	while (swapped)
	{
		swapped = FALSE;
		current = head;
		while (current->next != end)
		{
			if (cmp(current->key, current->next->key) > 0)
			{
				swap_env_fields(current, current->next);
				swapped = TRUE;
			}
			current = current->next;
		}
		end = current;
	}
	return (head);
}

//? temp->value == NULL case
static void	increment_shlvl(t_data *data)
{
	long long	lvl;
	t_env		*temp;

	lvl = 0;
	temp = data->env_copy;
	while (temp)
	{
		if (ft_strcmp(temp->key, "SHLVL") == 0)
		{
			lvl = ft_atoll(temp->value);
			free(temp->value);
			temp->value = ft_itoa((int)lvl + 1);
		}
		temp = temp->next;
	}
	temp = data->export;
	while (temp)
	{
		if (ft_strcmp(temp->key, "SHLVL") == 0)
		{
			free(temp->value);
			temp->value = ft_itoa(lvl + 1);
		}
		temp = temp->next;
	}
}

static void	add_to_export_list(t_data *data, char *key, char *value, t_env **export)
{
	t_env	*node;
	t_env	*last;

	node = malloc(sizeof(t_env));
	if (!node)
		return (report_error(data, "malloc", -1));
	node->key = key;
	if (value)
		node->equal = '=';
	else
		node->equal = '\0';
	node->value = value;
	node->next = NULL;
	if (!*export)
	{
		*export = node;
		if (data->export == NULL)
			data->export = *export;
		return ;
	}
	last = env_last_var(*export);
	last->next = node;
}

//? ft_strdup(temp->value) avec temp->value == NULL
void	init_export_list(t_data *data)
{
	t_env	*temp;
	t_env	*export;

	temp = data->env_copy;
	export = NULL;
	if (!temp)
	{
		add_to_export_list(data, ft_strdup("OLDPWD"), NULL, &export);
		add_to_export_list(data, ft_strdup("PWD"), getcwd(NULL, 0), &export);
		add_to_export_list(data, ft_strdup("SHLVL"), ft_strdup("1"), &export);
		data->export = export;
		return ;
	}
	while (temp)
	{
		if (!(temp->key[0] == '_' && temp->key[1] == '\0'))
			add_to_export_list(data, ft_strdup(temp->key),
				ft_strdup(temp->value), &export);
		temp = temp->next;
	}
	increment_shlvl(data);
	data->export = sort_export_list(export, ft_strcmp);
}
