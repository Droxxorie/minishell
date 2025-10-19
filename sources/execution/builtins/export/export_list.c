/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:00:47 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 13:48:39 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

t_env	*sort_export_list(t_env *head, int (*cmp)(const char *, const char *))
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
			lvl = parse_shlvl(temp->value);
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

static void	add_to_export_list(t_data *data, char *key, char *value,
		t_env **export)
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

static void	init_default_export_list(t_data *data, t_env **export)
{
	add_to_export_list(data, ft_strdup("OLDPWD"), NULL, export);
	add_to_export_list(data, ft_strdup("PWD"), getcwd(NULL, 0), export);
	add_to_export_list(data, ft_strdup("SHLVL"), ft_strdup("1"), export);
	data->export = *export;
}

void	init_export_list(t_data *data)
{
	t_env	*temp;
	t_env	*export;
	char	*value;

	if (data->export)
		free_env_list(&data->export);
	temp = data->env_copy;
	export = NULL;
	if (!temp)
		return (init_default_export_list(data, &export));
	while (temp)
	{
		value = NULL;
		if (temp->value)
			value = ft_strdup(temp->value);
		if (!(temp->key[0] == '_' && temp->key[1] == '\0'))
			add_to_export_list(data, ft_strdup(temp->key), value, &export);
		else
			free(value);
		temp = temp->next;
	}
	increment_shlvl(data);
	data->export = sort_export_list(export, ft_strcmp);
}
