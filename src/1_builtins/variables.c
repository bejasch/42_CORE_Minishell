/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:43:12 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 16:56:15 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

//This function apprends new variables to our env list.
char	**update_list(char *variable, char **envs)
{
	char	**current;
	char	*equal_sign;

	current = envs;
	while (*current)
	{
		equal_sign = ft_strchr(*current, '=');
		if (equal_sign && !ft_strncmp(*current, variable, equal_sign - *current)
			&& variable[equal_sign - *current] == '=')
		{
			free(*current);
			*current = ft_strdup(variable);
			if (!*current)
				return (NULL);
			return (envs);
		}
		current++;
	}
	*current = ft_strdup(variable);
	if (!*current)
		return (NULL);
	return (envs);
}

void	free_node(t_list *temp)
{
	free(temp->content);
	free(temp);
}

//This function deletes variables from our variables list (expanding).
void	delete_var(char *var, t_data *data)
{
	t_list	*temp;
	t_list	*prev;
	char	*str;
	int		i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	prev = NULL;
	temp = data->export_list;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		if (!ft_strncmp(str, var, i) && (!str[i] || str[i] == '='))
		{
			if (prev)
				prev->next = temp->next;
			else
				data->export_list = temp->next;
			free_node(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

//Function searches for a key and returns its value in list of variables.
char	*return_value_var(char *variable, t_list *exp_list)
{
	t_list	*temp;
	char	*str;
	int		i;

	temp = exp_list;
	while (temp)
	{
		str = (char *)temp->content;
		i = 0;
		while (str[i] && str[i] != '=')
			i++;
		if (!ft_strncmp(str, variable, i) && !variable[i])
			return (&variable[i + 1]);
		temp = temp->next;
	}
	return (NULL);
}

//Function created for testing purposes.
char	*search(char *variable, char **list, t_list *head)
{
	char	*out;

	out = return_value_env(variable, list);
	if (out)
		return (out);
	out = return_value_var(variable, head);
	if (out)
		return (out);
	return (NULL);
}
