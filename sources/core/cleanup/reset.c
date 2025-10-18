/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:46:48 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 19:31:01 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	reset_command_context(t_data *data)
{
	if (!data)
		return ;
	if (data->line_base)
	{
		free(data->line_base);
		data->line_base = NULL;
	}
	data->line = NULL;
	free_tokens(data);
	free_commands(data);
	if (data->pipes)
		free_pipes_all(data);
	data->saw_empty_word_as_command = FALSE;
	data->emit_empty_word = FALSE;
	data->tmp_in_redir_context = FALSE;
	data->tmp_have_content_before = FALSE;
	data->tmp_dollar_index = -1;
}
