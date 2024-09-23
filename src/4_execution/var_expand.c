/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:08:13 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/18 15:49:27 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static t_vars	*var_val(char *str, t_data *data, char in_sgl, char in_dbl);
static void		append_replace(char *str, t_vars **list, t_data *data);
static char		get_alphaval(char *str, t_vars **list, t_data *data);

char	expand_variables(t_list *linked_args, t_data *data)
{
	t_vars	*vars;

	if (!linked_args)
		return (0);
	while (linked_args)
	{
		vars = var_val(linked_args->content, data, 0, 0);
		if (vars && !replace_vars(linked_args, vars, linked_args->content))
			return (free_vars(vars), 0);
		free_vars(vars);
		linked_args = linked_args->next;
	}
	return (1);
}

// Finds variables in str to get replaced
// $ with alphanum, ? (last status), 0 (shellname)
static t_vars	*var_val(char *str, t_data *data, char in_sgl, char in_dbl)
{
	t_vars	*to_replace;

	to_replace = NULL;
	while (*str)
	{
		if (!in_sgl && !in_dbl && *str == '\'')
			in_sgl = 1;
		else if (!in_sgl && !in_dbl && *str == '"')
			in_dbl = 1;
		else if (in_sgl && *str == '\'')
			in_sgl = 0;
		else if (in_dbl && *str == '"')
			in_dbl = 0;
		else if (!in_sgl && *str == '$' && (ft_isalnum(*(str + 1))
				|| *(str + 1) == '?'))
			append_replace(str++, &to_replace, data);
		str++;
	}
	return (to_replace);
}

// fills the linked vars list (li) with infos about VARS to replace
// key_start holds pointer to $ from VAR
// key_len has the len of VAR-string including the $
// value_start holds pointer to the replace-string or NULL (!)
// len_diff has the len-difference after replacing (can be negative)
static void	append_replace(char *str, t_vars **li, t_data *data)
{
	while (*li)
		li = &(*li)->next;
	*li = (t_vars *)malloc(sizeof(t_vars));
	if (!*li)
		return ;
	(*li)->next = NULL;
	(*li)->key_start = str++;
	if (ft_isdigit(*str) || *str == '?')
		(*li)->key_len = 2;
	if (*str == '0')
		(*li)->value_start = data->shell_name;
	else if (ft_isdigit(*str))
		(*li)->value_start = NULL;
	else if (*str == '?')
		(*li)->value_start = data->status_str;
	else if (!get_alphaval(ft_strdup(str), li, data))
		return ;
	if (!(*li)->value_start)
		(*li)->len_diff = 0 - (*li)->key_len;
	else
		(*li)->len_diff = (int)ft_strlen((*li)->value_start) - (*li)->key_len;
}

// Finds the len and value of the variable from str
static char	get_alphaval(char *str, t_vars **list, t_data *data)
{
	if (!str)
	{
		free(*list);
		*list = NULL;
		return (0);
	}
	(*list)->key_len = 0;
	while (ft_isalnum(str[(*list)->key_len]))
		(*list)->key_len++;
	str[(*list)->key_len++] = '\0';
	(*list)->value_start = search(str, data->list_envs, NULL);
	free(str);
	return (1);
}
