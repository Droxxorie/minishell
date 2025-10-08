/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:46:48 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 14:47:00 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	reset_command_context(t_data *data)
{
	if (!data)
		return ;
	free_commands(data);
	free_tokens(data);
	if (data->pipes)
		free_pipes_all(data);
}
