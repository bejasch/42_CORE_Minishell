/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:41:45 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/17 23:09:25 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

//This function deletes variables from our env list.
char	**delete_env(char *variable, char **list)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (!(ft_strncmp(list[i], variable, j)) && !variable[j])
		{
			free(list[i]);
			while (list[i + 1])
			{
				list[i] = list[i + 1];
				i++;
			}
			list[i] = NULL;
			break ;
		}
		i++;
	}
	return (list);
}

//Function searches for a key and returns its value in the env list.
char	*search_env(char *variable, char **list)
{
	char	*out;

	out = return_value_env(variable, list);
	if (!out)
		return (NULL);
	return (out);
}

//Search function which loops through our env list.
char	*return_value_env(char *variable, char **list)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (!(ft_strncmp(list[i], variable, j)) && !variable[j])
			return (&list[i][j + 1]);
		i++;
	}
	return (NULL);
}

// prints out the current environments (saved in list_envs)
int	builtin_env(char **list_envs)
{
	while (*list_envs)
		ft_printf("%s\n", *(list_envs++));
	return (0);
}
