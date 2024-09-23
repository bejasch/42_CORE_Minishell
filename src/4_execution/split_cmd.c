/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:24:16 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/17 12:22:14 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	*end_of_arg(char *str, char in_sgl, char in_dbl);

// splits the cmd-sting into arguments, caring for single/double parenthesis
t_list	*get_args(char *str)
{
	t_list	*args_out;
	t_list	**writer;
	char	*tmp;
	size_t	len;

	if (!str)
		return (NULL);
	args_out = NULL;
	writer = &args_out;
	while (*str)
	{
		len = end_of_arg(str, 0, 0) - str;
		tmp = ft_substr(str, 0, len);
		if (!tmp)
			return (ft_lstclear(&args_out, free), NULL);
		*writer = ft_lstnew(tmp);
		if (!*writer)
			return (free(tmp), ft_lstclear(&args_out, free), NULL);
		writer = &(*writer)->next;
		str += len;
		while (*str == ' ')
			str++;
	}
	*writer = NULL;
	return (args_out);
}

// gives back the addres of the following arguments end 
static char	*end_of_arg(char *str, char in_sgl, char in_dbl)
{
	while (*str)
	{
		if (*str == '\0')
			break ;
		else if (in_sgl || in_dbl)
		{
			if (in_sgl && *str == '\'')
				in_sgl = 0;
			else if (in_dbl && *str == '"')
				in_dbl = 0;
		}
		else if (*str == '\'')
			in_sgl = 1;
		else if (*str == '"')
			in_dbl = 1;
		else if (*str == ' ')
			break ;
		str++;
	}
	return (str);
}
