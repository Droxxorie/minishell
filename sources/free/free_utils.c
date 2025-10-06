/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:52:23 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 01:24:33 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	minilist_clear(t_minilist **list)
{
	t_minilist	*next;

	if (!list)
		return ;
	while (*list)
	{
		next = (*list)->next;
		free((*list)->content);
		free(*list);
		*list = next;
	}
}
