/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_segment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 12:54:18 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 15:23:42 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	analyze_redir_context(t_data *data, char *line, size_t *i)
{
	int		j;
	size_t	k;

	if (line[*i] == '$' && quote_state(line, *i) != SQ)
	{
		data->tmp_in_redir_context = FALSE;
		data->tmp_have_content_before = FALSE;
		data->tmp_dollar_index = *i;
		j = (int)(*i) - 1;
		while (j >= 0 && ft_iswhitespace((unsigned char)line[j]))
			j--;
		if (j >= 0 && (line[j] == '<' || line[j] == '>'))
		{
			if (!(line[j] == '<' && j - 1 >= 0 && line[j - 1] == '<'))
			{
				data->tmp_in_redir_context = TRUE;
				k = (size_t)j + 1;
				while (k < *i && ft_iswhitespace((unsigned char)line[k]))
					k++;
				if (k < *i)
					data->tmp_have_content_before = TRUE;
			}
		}
	}
}

char	*expand_segment(t_data *data, char *line, size_t *i,
		char *accumulated_line)
{
	char	*variable;

	analyze_redir_context(data, line, i);
	if (line[*i] == '$' && quote_state(line, *i) != SQ && line[*i + 1] == '?')
	{
		variable = ft_strdup("$?");
		if (!variable)
			return (report_error(data, "strdup", 1), NULL);
		return (expand_variable(data, variable, i, accumulated_line));
	}
	if (line[*i] == '$' && quote_state(line, *i) == NO_QUOTE
		&& (line[*i + 1] == '"' || line[*i + 1] == '\''))
		return ((*i)++, accumulated_line);
	if (line[*i] == '$' && quote_state(line, *i) != SQ)
	{
		variable = extract_var_name(line + *i, i);
		if (!variable)
			return (append_current_char(data, line, i, accumulated_line));
		return (expand_variable(data, variable, i, accumulated_line));
	}
	return (append_current_char(data, line, i, accumulated_line));
}
