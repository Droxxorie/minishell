/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:51:56 by eraad             #+#    #+#             */
/*   Updated: 2025/09/24 18:53:18 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_data *data)
{
	t_token *temp;

	if (!data || !data->tokens)
		return ;
	while (data->tokens)
	{
		temp = data->tokens;
		data->tokens = data->tokens->next;
		if (temp->value)
		{
			free(temp->value);
			temp->value = NULL;
		}
		if (temp)
			free(temp);
		temp = NULL;
	}
}
