/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/18 20:17:02 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

volatile __sig_atomic_t	g_signal;

static void	check_for_signals(t_data *data);
static char	handle_input(t_data *data);

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	if (!initialize_data(&data, argc, envp))
		exit (1);
	while (1)
	{
		data.input = readline("minishell > ");
		check_for_signals(&data);
		if (!data.input)
			break ;
		handle_signals(2);
		add_history(data.input);
		if (!handle_input(&data))
			continue ;
		if (build_ast(&data))
			data.status = parse_ast(data.ast_root, &data);
		free_prompt_data(&data);
	}
	ft_printf("exit\n");
	return (ft_free(&data, data.status));
}

// Checks for signals and updates the status accordingly
static void	check_for_signals(t_data *data)
{
	if (g_signal)
	{
		data->status = 130;
		if (data->status_str)
			free(data->status_str);
		data->status_str = ft_itoa(data->status);
		g_signal = 0;
	}
}

static char	handle_input(t_data *data)
{
	if (!check_input(data->input, 0))
	{
		free(data->input);
		data->status = 2;
		if (data->status_str)
			free(data->status_str);
		data->status_str = ft_itoa(data->status);
		return (0);
	}
	return (1);
}
