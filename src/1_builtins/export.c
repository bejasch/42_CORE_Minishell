/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:43:42 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/23 16:50:41 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// export list is linked list
t_list	*update_exportlist(char *new_var, t_list *exp_list)
{
	t_list	*temp;
	char	*str;
	int		i;

	i = 0;
	while (new_var[i] && new_var[i] != '=')
		i++;
	temp = exp_list;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		if (!ft_strncmp(str, new_var, i) && (!str[i] || str[i] == '='))
		{
			if (!new_var[i])
				return (exp_list);
			free (temp->content);
			temp->content = ft_strdup(new_var);
			return (exp_list);
		}
		temp = temp->next;
	}
	append_node(&exp_list, new_var);
	return (exp_list);
}

int	is_valid_identifier(const char *arg)
{
	int	i;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **argv, int argc, char **list_envs, t_list *export_list)
{
	int	i;
	int	invalid;

	invalid = 0;
	if (argc == 1)
		print_list(export_list);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			error_message("export", argv[i], "not a valid identifier");
			invalid = 1;
		}
		else if (with_value(argv[i]))
		{
			update_list(argv[i], list_envs);
			update_exportlist(argv[i], export_list);
		}
		else
			update_exportlist(argv[i], export_list);
		i++;
	}
	return (invalid);
}
