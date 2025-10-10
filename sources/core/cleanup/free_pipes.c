/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:46:27 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 23:02:06 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_pipes_all(t_data *data)
{
	int	count;

	if (!data || !data->pipes)
		return ;
	if (data->pipes->nb > 0 && data->pipes->fds)
	{
		count = 2 * (data->pipes->nb);
		close_pipe_fds(data->pipes->fds, count);
		free(data->pipes->fds);
	}
	if (data->pipes->pids)
		free(data->pipes->pids);
	free(data->pipes);
	data->pipes = NULL;
}
