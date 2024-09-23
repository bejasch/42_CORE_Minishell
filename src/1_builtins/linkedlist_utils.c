/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linkedlist_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:44:00 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 17:34:32 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_list	*create_node(const char *str)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup(str);
	if (!new_node->content)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

t_list	*create_nodeexport(const char *str)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = ft_strdup(str);
	if (!node->content)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

// TODO: Maybe do it larger!?!
char	**create_list(char **list)
{
	list = malloc(sizeof(char *) * 400);
	if (!list)
		return (NULL);
	return (list);
}

void	append_node(t_list **head, const char *str)
{
	t_list	*new_node;
	t_list	**temp;

	new_node = create_node(str);
	if (!new_node)
		return ;
	if (!*head)
		*head = new_node;
	else
	{
		temp = head;
		while (*temp)
			temp = &(*temp)->next;
		*temp = new_node;
	}
}

//Function to create the second "export" list
// Uses the env list to create
// Node and content is new malloced
t_list	*array_to_linkedlist(char **env)
{
	t_list	*head;
	t_list	*current;
	int		i;

	if (env[0] == NULL)
		return (NULL);
	head = create_nodeexport(env[0]);
	current = head;
	i = 1;
	while (env[i] != NULL)
	{
		current->next = create_nodeexport(env[i]);
		current = current->next;
		i++;
	}
	return (head);
}
