/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:17:48 by eraad             #+#    #+#             */
/*   Updated: 2025/09/15 16:24:25 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	t_env *new_var;
	t_env *temp;

	if (!key || !value)
		return (EXIT_FAILURE);
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (EXIT_FAILURE);
	new_var->key = key;
	new_var->equal = '=';
	new_var->value = value;
	new_var->next = NULL;
	if (!*env)
	{
		*env = new_var;
		if (data != NULL)
			data->env_copy = *env;
		return (EXIT_SUCCESS);
	}
	temp = env_last_var(*env);
	temp->next = new_var;
	return (EXIT_SUCCESS);
}
