/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:13:40 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:27:22 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*append_current_char(t_data *data, char *line, size_t *i,
		char *accumulated_line)
{
	size_t	len;
	char	*temp;

	if (!accumulated_line)
		len = 0;
	else
		len = ft_strlen(accumulated_line);
	temp = malloc(sizeof(char) * (len + 2));
	if (!temp)
		return (report_error(data, "malloc", 1), NULL);
	if (accumulated_line)
	{
		ft_memcpy(temp, accumulated_line, len);
		free(accumulated_line);
	}
	temp[len] = line[*i];
	temp[len + 1] = '\0';
	(*i)++;
	return (temp);
}

static char	*expand_variable(t_data *data, char *variable, size_t *i,
		char *accumulated_line)
{
	char	*current_line;
	char	*expanded_value;

	current_line = ft_strdup(accumulated_line);
	if (!current_line)
		return (report_error(data, "strdup", 1), NULL);
	if (ft_strcmp(variable, "$?") == 0)
	{
		expanded_value = ft_itoa(data->exit_status);
		(*i) += 2;
	}
	else if (env_var_exists(data, variable))
		expanded_value = get_env_value(data, variable);
	else
		expanded_value = ft_calloc(1, 1);
	free(variable);
	free(accumulated_line);
	accumulated_line = ft_strjoin(current_line, expanded_value);
	free(current_line);
	free(expanded_value);
	if (!accumulated_line)
		return (report_error(data, "strjoin", 1), NULL);
	return (accumulated_line);
}

static char	*expand_segment(t_data *data, char *line, size_t *i,
		char *accumulated_line)
{
	char	*variable;

	if (line[*i] == '$' && line[*i + 1] == '?' && quote_state(line, *i) != SQ)
	{
		variable = ft_strdup("$?");
		if (!variable)
			return (report_error(data, "strdup", 1), NULL);
		return (expand_variable(data, variable, i, accumulated_line));
	}
	if (line[*i] == '$' && quote_state(line, *i) != SQ)
	{
		variable = extract_var_name(line + *i, i);
		if (!variable)
			return (append_current_char(data, line, i, accumulated_line));
		return (expand_variable(data, variable, i, accumulated_line));
	}
	return (append_current_char(data, line, i, accumulated_line));
}

static char	*expand_env_variables(t_data *data)
{
	size_t	i;
	char	*accumulated_line;
	char	*temp;

	i = 0;
	if (!data->line)
		return (NULL);
	accumulated_line = ft_strdup("");
	if (!accumulated_line)
		return (report_error(data, "strdup", 1), NULL);
	while (data->line[i])
	{
		temp = expand_segment(data, data->line, &i, accumulated_line);
		if (!temp)
		{
			free(accumulated_line);
			return (NULL);
		}
		accumulated_line = temp;
	}
	return (accumulated_line);
}

int	expander(t_data *data)
{
	char	*expanded_line;

	if (!data->line)
		return (EXIT_FAILURE);
	expanded_line = NULL;
	if (need_expansion(data->line) == TRUE)
	{
		expanded_line = expand_env_variables(data);
		if (!expanded_line)
			return (EXIT_FAILURE);
		free(data->line);
		data->line = expanded_line;
	}
	return (EXIT_SUCCESS);
}
