/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 17:45:18 by eraad             #+#    #+#             */
/*   Updated: 2025/09/29 16:49:43 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *extract_var_name(char *str, size_t *i)
{
	char *variable;
	size_t variable_len;

	if (!str || !*str)
		return (NULL);
	if (str[0] == '$' && str[1] == '$')
	{
		*i += 2;
		return (ft_strdup("$$"));
	}
	if (str[0] == '?' && (*i)++)
		return (ft_strdup("?"));
	variable_len = 0;
	while (str[variable_len] && (ft_isalnum(str[variable_len])
			|| str[variable_len] == '_'))
		variable_len++;
	if (variable_len == 0)
		return (NULL);
	variable = ft_substr(str, 1, variable_len);
	if (!variable)
		return (NULL);
	*i += variable_len;
	return (variable);
}

char *get_env_value(t_data *data, char *variable)
{
	char *expanded_value;
	t_env *temp;

	expanded_value = NULL;
	temp = data->env_copy;
	while (temp)
	{
		// if (ft_strcmp(temp->value, variable) == 0) //? wich one
		if (ft_strcmp(temp->key, variable) == 0)
		{
			expanded_value = ft_strdup(temp->value);
			if (!expanded_value)
				return (report_error(data, "strdup", 1), NULL);
			return (expanded_value);
		}
		temp = temp->next;
	}
	return (NULL);
}

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
		return (NO_QUOTE);
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
