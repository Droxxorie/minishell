/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:14:14 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:28:14 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

t_bool	env_var_exists(t_data *data, char *variable)
{
	t_env	*temp;

	temp = data->env_copy;
	while (temp)
	{
		if (ft_strcmp(temp->key, variable) == 0)
			return (TRUE);
		temp = temp->next;
	}
	return (FALSE);
}

char	*get_env_value(t_data *data, char *variable)
{
	char	*expanded_value;
	t_env	*temp;

	expanded_value = NULL;
	temp = data->env_copy;
	while (temp)
	{
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

char	*extract_var_name(char *str, size_t *i)
{
	char	*variable;
	size_t	len;

	if (!str || !*str)
		return (NULL);
	if (str[0] == '$' && str[1] == '$')
	{
		*i += 2;
		return (ft_strdup("$$"));
	}
	if (str[0] == '$' && str[1] == '?')
		return (NULL);
	len = 1;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	len--;
	if (len == 0)
		return (NULL);
	variable = ft_substr(str, 1, len);
	if (!variable)
		return (NULL);
	*i += len + 1;
	return (variable);
}
