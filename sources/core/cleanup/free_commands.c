/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:45:09 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 23:09:45 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_char_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
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

static void	redir_list_clear(t_redir **list)
{
	t_redir	*next;

	if (!list)
		return ;
	while (*list)
	{
		next = (*list)->next;
		if ((*list)->fd > 2)
			close((*list)->fd);
		if ((*list)->value)
			free((*list)->value);
		free(*list);
		*list = next;
	}
}

void	free_commands(t_data *data)
{
	t_command	*temp;

	if (!data)
		return ;
	while (data->commands)
	{
		temp = data->commands;
		data->commands = data->commands->next;
		if (temp->redirs)
			redir_list_clear(&temp->redirs);
		if (temp->command)
			free(temp->command);
		if (temp->argv)
			free_char_array(temp->argv);
		if (temp->args)
			minilist_clear(&temp->args);
		if (temp->flags)
			minilist_clear(&temp->flags);
		free(temp);
	}
}
