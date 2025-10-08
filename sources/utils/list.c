/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:12:26 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 16:13:01 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	swap_env_fields(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;
	char	temp_equal;

	if (!a || !b)
		return ;
	temp_key = a->key;
	temp_value = a->value;
	temp_equal = a->equal;
	a->key = b->key;
	a->value = b->value;
	a->equal = b->equal;
	b->key = temp_key;
	b->value = temp_value;
	b->equal = temp_equal;
}

void	minilist_add_back(t_minilist **list, t_minilist *new_node)
{
	t_minilist	*last;

	if (!list || !new_node)
		return ;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	last = minilist_last(*list);
	last->next = new_node;
}

t_minilist	*minilist_last(t_minilist *list)
{
	if (!list)
		return (NULL);
	while (list->next)
		list = list->next;
	return (list);
}
