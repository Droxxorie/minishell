/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:48:10 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:15:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	report_error3(const char *message_1, const char *message_2,
		const char *message_3)
{
	ft_putstr_fd("minishell: ", 2);
	if (message_1 && *message_1)
		ft_putstr_fd((char *)message_1, 2);
	if (message_2 && *message_2)
		ft_putstr_fd((char *)message_2, 2);
	if (message_3 && *message_3)
		ft_putstr_fd((char *)message_3, 2);
	ft_putstr_fd("\n", 2);
}

void	report_error2(const char *message_1, const char *message_2)
{
	ft_putstr_fd("minishell: ", 2);
	if (message_1 && *message_1)
		ft_putstr_fd((char *)message_1, 2);
	if (message_2 && *message_2)
		ft_putstr_fd((char *)message_2, 2);
	ft_putstr_fd("\n", 2);
}

void	report_error(t_data *data, const char *message, int exit_code)
{
	if (message && *message)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)message, 2);
		ft_putstr_fd("\n", 2);
	}
	if (exit_code != -1)
		data->exit_status = exit_code;
}
