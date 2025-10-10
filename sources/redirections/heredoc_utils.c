/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:31:44 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 19:49:14 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	append_exit_status(t_data *data, char **out, size_t *i)
{
	char	*temp;

	temp = NULL;
	temp = ft_itoa(data->exit_status);
	if (!temp)
		return (-1);
	if (append_join(out, temp) == -1)
		return (-1);
	*i += 2;
	return (0);
}

static int	append_variable(t_data *data, const char *s, size_t *i, char **out)
{
	size_t	start;
	char	*name;
	char	*val;

	start = *i + 1;
	name = NULL;
	val = NULL;
	while (ft_isalnum((unsigned char)s[start]) || s[start] == '_')
		start++;
	name = ft_substr(s, *i + 1, start - (*i + 1));
	if (!name)
		return (-1);
	val = get_env_value(data, name);
	free(name);
	if (val && append_join(out, val) == -1)
		return (-1);
	*i = start;
	return (0);
}

static int	append_plain_text(const char *s, size_t *i, char **out)
{
	size_t	start;
	char	*tmp;

	start = *i;
	while (s[*i] && s[*i] != '$')
		(*i)++;
	tmp = ft_substr(s, start, *i - start);
	if (!tmp)
		return (-1);
	if (append_join(out, tmp) == -1)
		return (-1);
	return (0);
}

static int	dispatch_dollar(t_data *data, const char *s, size_t *i, char **out)
{
	if (s[*i + 1] == '?')
		return (append_exit_status(data, out, i));
	if (ft_isalpha((unsigned char)s[*i + 1]) || s[*i + 1] == '_')
		return (append_variable(data, s, i, out));
	return (0);
}

char	*expand_heredoc_line(t_data *data, const char *s)
{
	size_t	i;
	char	*out;

	i = 0;
	out = NULL;
	out = ft_strdup("");
	if (!out)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '$')
		{
			if (dispatch_dollar(data, s, &i, &out) == -1)
				return (free(out), NULL);
			if (s[i])
				continue ;
		}
		if (append_plain_text(s, &i, &out) == -1)
			return (free(out), NULL);
	}
	return (out);
}
