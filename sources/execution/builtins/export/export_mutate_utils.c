/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_mutate_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 14:47:33 by eraad             #+#    #+#             */
/*   Updated: 2025/10/12 16:08:46 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

void	export_update_value(t_data *data, const char *key, const char *arg)
{
	t_env		*temp;
	const char	*offset;
	char		*new_value;

	temp = data->export;
	offset = ft_strchr(arg, '=');
	if (!offset)
		return ;
	new_value = ft_strdup(offset + 1);
	if (!new_value)
		return (report_error(data, "strdup", -1));
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			temp->equal = '=';
			return ;
		}
		temp = temp->next;
	}
	free(new_value);
}
