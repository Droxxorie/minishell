/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:13:40 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 15:05:24 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_current_char(t_data *data, char *line, size_t *i,
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

static int	append_non_expanded_char(t_data *data, size_t *i,
		char **accumulated_line)
{
	char	*buffer;

	buffer = append_current_char(data, data->line, i, *accumulated_line);
	if (!buffer)
		return (EXIT_FAILURE);
	*accumulated_line = buffer;
	return (EXIT_SUCCESS);
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
			if (append_non_expanded_char(data, &i,
					&accumulated_line) == EXIT_FAILURE)
				return (free(accumulated_line), NULL);
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
