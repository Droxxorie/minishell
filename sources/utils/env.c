/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:21:20 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 04:00:42 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_env_value(t_data *data, const char *key)
{
	const t_env	*temp;

	if (!data || !data->env_copy || !key)
		return (NULL);
	temp = data->env_copy;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}
