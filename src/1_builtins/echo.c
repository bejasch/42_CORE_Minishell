/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:26:12 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 18:26:13 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// prints out the arguments passed to it
// includes the option -n, which suppresses the trailing newline
int	builtin_echo(char **argv, int argc)
{
	int	i;
	int	option_n;

	option_n = 0;
	if (argc > 1 && ft_strncmp(argv[1], "-n", 3) == 0)
	{
		option_n = 1;
		i = 2;
	}
	else
		i = 1;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!option_n)
		write(1, "\n", 1);
	return (0);
}
