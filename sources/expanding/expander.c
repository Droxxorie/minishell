/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 16:44:06 by eraad             #+#    #+#             */
/*   Updated: 2025/09/28 18:58:12 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *expand_variable(t_data *data, char *variable, size_t *i, char *accumulated_line)
{
	char *current_line;
	char *expanded_value;

	current_line = ft_strdup(accumulated_line);
	if (!current_line)
		return (report_error(data, "strdup", 1), NULL);
	if (ft_strcmp(variable, "$?") == 0)
	{
		expanded_value = ft_itoa(data->exit_status);
		(*i) += 2;
	}
	else if (env_var_exists(data, variable))
		expanded_value = give_me_inside_var(variable, data); // TODO
	else
		expanded_value = ft_strdup("");
	free(variable);
	free(accumulated_line);
	accumulated_line = ft_concatenate(current_line, expanded_value); // TODO
	free(current_line);
	free(expanded_value);
	if (!accumulated_line)
		return (report_error(data, "strdup", 1), NULL);
	return (accumulated_line);
}

static char	*expand_segment(t_data *data, char *line, size_t *i, char *accumulated_line)
{
	char	*variable;

	if (line[*i] == '$' && line[*i + 1] == '?' && quote_state(line, *i) != SQ)
	{
		variable = ft_strdup("$?");
		if (!variable)
			return (report_error(data, "strdup", 1), NULL);
		return (expand_var_or_exit(data, variable, i, accumulated_line)); // TODO
	}
	if (line[*i] == '$' && quote_state(line, *i) != SQ)
	{
		variable = extract_var(line + *i, i); // TODO
		if (!variable)
			return (report_error(data, "Failed to extract variable", 1), NULL);
		return (expand_var_or_exit(data, variable, i)); // TODO
	}
	return (process_character(data, line, i)); // TODO
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
		temp = expand_segment(data, data->line, &i, accumulated_line); // TODO
		if (!temp)
		{
			free(accumulated_line);
			return (NULL);
		}
		accumulated_line = temp;
		//? free temp ?
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
		expanded_line = expand_env_variables(data); // TODO
		if (!expanded_line)
			return (EXIT_FAILURE);
		free(data->line);
		data->line = expanded_line;
	}
	return (EXIT_SUCCESS);
}
