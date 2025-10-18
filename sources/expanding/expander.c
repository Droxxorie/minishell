/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:13:40 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 19:50:06 by eraad            ###   ########.fr       */
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

static char	*expand_variable(t_data *data, char *variable, size_t *i,
		char *accumulated_line)
{
	char	*current_line;
	char	*expanded_value;
	t_bool	is_empty;
	t_bool	has_spaces;

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
	// --- NEW: gestion "ambiguous redirect" simple
	if (data->tmp_in_redir_context && quote_state(data->line,
			data->tmp_dollar_index) == NO_QUOTE)
	{
		is_empty = (!expanded_value || expanded_value[0] == '\0');
		has_spaces = (!is_empty && has_unquoted_space(expanded_value));
		// ambigu si vide ET aucun contenu avant le $,
		// ou si ça introduit plusieurs mots
		if ((is_empty && !data->tmp_have_content_before) || has_spaces)
		{
			// message simple, sans nom de variable fautive
			report_error(NULL, "ambiguous redirect", -1);
			data->exit_status = 1;
			free(variable);
			// free(accumulated_line);
			free(current_line);
			free(expanded_value);
			// reset flags pour éviter effets de bord
			data->tmp_in_redir_context = FALSE;
			data->tmp_have_content_before = FALSE;
			data->tmp_dollar_index = 0;
			return (NULL); // on fait échouer l’expander
		}
	}
	// reset des flags si non-ambigu
	data->tmp_in_redir_context = FALSE;
	data->tmp_have_content_before = FALSE;
	data->tmp_dollar_index = 0;
	// --- FIN NEW
	free(variable);
	free(accumulated_line);
	accumulated_line = strjoin_free_both(current_line, expanded_value);
	if (!accumulated_line)
		return (report_error(data, "strjoin", 1), NULL);
	return (accumulated_line);
}

static char	*expand_segment(t_data *data, char *line, size_t *i,
		char *accumulated_line)
{
	t_quote	qs;
	char	*variable;
	ssize_t	j;
	size_t	k;

	qs = quote_state(line, *i);
	// --- NEW CODE BLOCK ---
	if (line[*i] == '$' && quote_state(line, *i) != SQ)
	{
		// --- NEW: calcul du contexte redirection autour du '$'
		data->tmp_in_redir_context = FALSE;
		data->tmp_have_content_before = FALSE;
		data->tmp_dollar_index = *i;
		// scan à gauche pour trouver le dernier non-espace
		j = (ssize_t)(*i) - 1;
		while (j >= 0 && ft_iswhitespace((unsigned char)line[j]))
			j--;
		if (j >= 0 && (line[j] == '<' || line[j] == '>'))
		{
			// exclure heredoc '<<' (bash: pas d’ambiguïté ici)
			if (!(line[j] == '<' && j - 1 >= 0 && line[j - 1] == '<'))
			{
				data->tmp_in_redir_context = TRUE;
				// y avait-il déjà du contenu entre l’opérateur et le '$' ?
				k = (size_t)j + 1;
				while (k < *i && ft_iswhitespace((unsigned char)line[k]))
					k++;
				if (k < *i)
					data->tmp_have_content_before = TRUE;
			}
		}
	} // --- END NEW
	if (line[*i] == '$' && qs != SQ && line[*i + 1] == '?')
	{
		variable = ft_strdup("$?");
		if (!variable)
			return (report_error(data, "strdup", 1), NULL);
		return (expand_variable(data, variable, i, accumulated_line));
	}
	if (line[*i] == '$' && qs == NO_QUOTE && (line[*i + 1] == '"' || line[*i
			+ 1] == '\''))
		return ((*i)++, accumulated_line);
	if (line[*i] == '$' && qs != SQ)
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
	size_t	before;

	i = 0;
	if (!data->line)
		return (NULL);
	accumulated_line = ft_strdup("");
	if (!accumulated_line)
		return (report_error(data, "strdup", 1), NULL);
	while (data->line[i])
	{
		before = i;
		temp = expand_segment(data, data->line, &i, accumulated_line);
		if (!temp)
			return (free(accumulated_line), NULL);
		accumulated_line = temp;
		if (i == before)
		{
			temp = append_current_char(data, data->line, &i, accumulated_line);
			if (!temp)
				return (free(accumulated_line), NULL);
			accumulated_line = temp;
		}
	}
	return (accumulated_line);
}

int	expander(t_data *data)
{
	char	*expanded_line;

	if (!data->line)
		return (EXIT_FAILURE);
	expanded_line = NULL;
	if (need_expansion(data->line) == FALSE)
		return (EXIT_SUCCESS);
	expanded_line = expand_env_variables(data);
	if (!expanded_line)
		return (EXIT_FAILURE);
	if (data->line_base)
		free(data->line_base);
	data->line_base = expanded_line;
	data->line = expanded_line;
	return (EXIT_SUCCESS);
}
