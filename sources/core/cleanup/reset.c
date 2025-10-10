/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:46:48 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 19:05:32 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	reset_command_context(t_data *data)
{
	if (!data)
		return ;
	free_tokens(data);
	free_commands(data);
	if (data->pipes)
		free_pipes_all(data);
}
