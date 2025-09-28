/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:45:18 by eraad             #+#    #+#             */
/*   Updated: 2025/09/28 18:57:53 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_bool env_var_exists(t_data *data, char *variable)
{
	t_env *temp;

	temp = data->env_copy;
	while (temp)
	{
		if (ft_strcmp(temp->key, variable) == 0)
			return (TRUE);
		temp = temp->next;
	}
	return (FALSE);
}

t_bool	need_expansion(char *str)
{
	size_t	i;
	t_quote	state;

	i = 0;
	state = NO_QUOTE;
	while (str[i])
	{
		state = quote_state(str, i);
		if (str[i] == '$' && state != SINGLE_QUOTE)
			if (str[i + 1] && (str[i + 1] == '$' || str[i + 1] == '?' || str[i
					+ 1] == '_' || ft_isalnum(str[i + 1])))
				return (TRUE);
		i++;
	}
	return (FALSE);
}

t_quote	quote_state(char *line, size_t index)
{
	t_quote	state;
	size_t	i;

	state = NO_QUOTE;
	i = 0;
	if (!line)
		return (NULL);
	while (line[i] && i < index)
	{
		if (line[i] == '"' && state != SINGLE_QUOTE && state != DOUBLE_QUOTE)
			state = DOUBLE_QUOTE;
		else if (line[i] == '"' && state == DOUBLE_QUOTE)
			state = NO_QUOTE;
		else if (line[i] == '\'' && state != DOUBLE_QUOTE
			&& state != SINGLE_QUOTE)
			state = SINGLE_QUOTE;
		else if (line[i] == '\'' && state == SINGLE_QUOTE)
			state = NO_QUOTE;
		i++;
	}
	return (state);
}
