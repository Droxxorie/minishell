/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:48:10 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 22:38:24 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void put_stderr_3(const char *m1, const char *m2, const char *m3)
{
	if (m1 && *m1)
		ft_putstr_fd((char *)m1, 2);
	if (m2 && *m2)
		ft_putstr_fd((char *)m2, 2);
	if (m3 && *m3)
		ft_putstr_fd((char *)m3, 2);
	ft_putstr_fd("\n", 2);
}

void	report_error3(const char *message_1, const char *message_2,
		const char *message_3)
{
	ft_putstr_fd("minishell: ", 2);
	put_stderr_3(message_1, message_2, message_3);
}

void	report_error2(const char *message_1, const char *message_2)
{
	put_stderr_3("minishell: ", message_1, message_2);
}

void	report_error(t_data *data, const char *message, int exit_code)
{
	if (message)
	{
		put_stderr_3("minishell: ", message, NULL);
	}
	if (exit_code != -1)
		data->exit_status = exit_code;
}
