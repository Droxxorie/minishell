/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:39:01 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 23:10:42 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
