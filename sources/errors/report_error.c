/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:10:20 by eraad             #+#    #+#             */
/*   Updated: 2025/09/28 18:28:32 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	report_error(t_data *data, const char *message, int exit_code)
{
	if (message && *message)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(message, 2);
		ft_putstr_fd("\n", 2);
	}
	data->exit_status = exit_code;
}
