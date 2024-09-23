/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:39:34 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 18:39:36 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static size_t	len_after_replace(char *original, t_vars *vars);

char	replace_vars(t_list *arg, t_vars *vars, char *content_ptr)
{
	char	*new;
	char	*cpy;

	new = (char *)malloc(len_after_replace(arg->content, vars) * sizeof(char));
	if (!new)
		return (0);
	cpy = new;
	while (*content_ptr)
	{
		if (vars && content_ptr == vars->key_start)
		{
			if (vars->value_start)
				while (*vars->value_start)
					*(cpy++) = *(vars->value_start++);
			content_ptr += vars->key_len;
			vars = vars->next;
		}
		else
			*(cpy++) = *(content_ptr++);
	}
	*cpy = '\0';
	free(arg->content);
	arg->content = new;
	return (1);
}

static size_t	len_after_replace(char *original, t_vars *vars)
{
	size_t	len;

	len = ft_strlen(original);
	while (vars)
	{
		len += vars->len_diff;
		vars = vars->next;
	}
	return (len + 1);
}

// Frees the linked vars list, but not the content, which is extern 
void	free_vars(t_vars *vars)
{
	t_vars	*tmp;

	while (vars)
	{
		tmp = vars;
		vars = vars->next;
		free(tmp);
	}
}
