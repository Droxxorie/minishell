/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:29:40 by eraad             #+#    #+#             */
/*   Updated: 2025/10/03 22:46:07 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env_list(t_env **env)
{
	t_env	*current;
	t_env	*next;

	if (!env || !*env)
		return ;
	current = *env;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}

void	free_redirections(t_data *data)
{
	if (!data)
		return ;
	if (data->input.fd > 2)
		close(data->input.fd);
	data->input.type = REDIR_IN;
	if (data->input.value)
	{
		free(data->input.value);
		data->input.value = NULL;
	}
	data->input.fd = STDIN_FILENO;
	data->input.quote = NQ;
	if (data->output.fd > 2)
		close(data->output.fd);
	data->output.type = REDIR_OUT;
	if (data->output.value)
	{
		free(data->output.value);
		data->output.value = NULL;
	}
	data->output.fd = STDOUT_FILENO;
	data->output.quote = NQ;
}

void	free_commands(t_data *data)
{
	t_command *temp;

	while (data && data->commands)
	{
		temp = data->commands;
		data->commands = data->commands->next;
		temp->next = NULL;
		if (temp->command)
		{
			free(temp->command);
			temp->command = NULL;
		}
		if (temp->argv)
			free_char_array(temp->argv);
		if (temp->args)
			ft_lstclear(&temp->args, free);
		if (temp->flags)
			ft_lstclear(&temp->flags, free);
		free(temp);
		temp = NULL;
	}
}

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

void	free_tokens(t_data *data)
{
	t_token	*temp;

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
