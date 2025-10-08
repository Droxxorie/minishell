/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:46:27 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 14:46:38 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_pipes_all(t_data *data)
{
	if (data && data->pipes)
	{
		free(data->pipes->fds);
		free(data->pipes->pids);
		free(data->pipes);
		data->pipes = NULL;
	}
}
