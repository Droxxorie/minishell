/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:49:16 by eraad             #+#    #+#             */
/*   Updated: 2025/10/12 13:27:02 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	scan_line(char *line, int opened_quote, int index, t_data *data)
{
	while (line[index])
	{
		if (!opened_quote && (line[index] == '\'' || line[index] == '"'))
		{
			data->which_quote = line[index];
			opened_quote = 1;
		}
		else if ((line[index] == '\'' || line[index] == '"')
			&& line[index] == data->which_quote)
		{
			opened_quote = 0;
			data->which_quote = '\0';
		}
		if ((line[index] == '\n' || line[index] == ';' || line[index] == '\\')
			&& !opened_quote)
			return (print_syntax_error(line[index], 1), 1);
		if (!opened_quote && line[index] == '|' && line[index + 1] == '|')
			return (print_syntax_error('|', 2), 2);
		if ((line[index] == '&' && line[index + 1] == '&') && !opened_quote)
			return (print_syntax_error('&', 3), 3);
		index++;
	}
	if (opened_quote)
		return (print_syntax_error(data->which_quote, 4), 4);
	return (0);
}

int	syntax_error_handler(t_data *data)
{
	int	errno;

	if (!data->line)
		return (0);
	data->which_quote = '\0';
	errno = scan_line(data->line, 0, 0, data);
	if (errno != 0)
	{
		data->exit_status = 2;
		return (1);
	}
	return (0);
}

void	print_syntax_error(char error, int code)
{
	if (code < 4)
	{
		ft_putstr_fd("minishell: operation '", 2);
		if (code == 1)
			ft_putchar_fd(error, 2);
		else if (code == 2)
			ft_putstr_fd("||", 2);
		else if (code == 3)
			ft_putstr_fd("&&", 2);
		ft_putstr_fd("' not supported\n", 2);
	}
	else if (code == 4 || code == 5)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(error, 2);
		ft_putstr_fd("'\n", 2);
		if (code == 5)
			ft_putstr_fd("invalid heredoc limiter\n", 2);
	}
	else if (code == 6)
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
	else if (code == 7)
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
}
