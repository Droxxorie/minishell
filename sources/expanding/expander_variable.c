/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 12:44:25 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 15:44:40 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_bool	has_unquoted_space(const char *s)
{
	while (*s)
	{
		if (ft_iswhitespace((unsigned char)*s))
			return (TRUE);
		s++;
	}
	return (FALSE);
}

static t_bool	check_ambiguous_redirect(t_data *data, char *current_line,
		char *expanded_value, char *variable)
{
	t_bool	is_empty;
	t_bool	has_spaces;

	if (data->tmp_in_redir_context && quote_state(data->line,
			data->tmp_dollar_index) == NO_QUOTE)
	{
		is_empty = (!expanded_value || expanded_value[0] == '\0');
		has_spaces = (!is_empty && has_unquoted_space(expanded_value));
		if ((is_empty && !data->tmp_have_content_before) || has_spaces)
		{
			report_error(data, "ambiguous redirect", 1);
			free(variable);
			free(current_line);
			free(expanded_value);
			data->tmp_in_redir_context = FALSE;
			data->tmp_have_content_before = FALSE;
			data->tmp_dollar_index = 0;
			return (TRUE);
		}
	}
	data->tmp_in_redir_context = FALSE;
	data->tmp_have_content_before = FALSE;
	data->tmp_dollar_index = 0;
	return (FALSE);
}

char	*expand_variable(t_data *data, char *variable, size_t *i,
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
	if (check_ambiguous_redirect(data, current_line, expanded_value,
			variable) == TRUE)
		return (NULL);
	free(variable);
	free(accumulated_line);
	accumulated_line = strjoin_free_both(current_line, expanded_value);
	if (!accumulated_line)
		return (report_error(data, "strjoin", 1), NULL);
	return (accumulated_line);
}
