/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:04:25 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 17:39:22 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// Util function for the export built-in. Sorts the list in alphabetical order.
void	sort_list(t_list *head)
{
	t_list	*i;
	t_list	*j;
	char	*temp;

	if (head == NULL)
	{
		return ;
	}
	i = head;
	while (i->next != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			if (ft_strncmp(i->content, j->content,
					ft_strlen(i->content) + 1) > 0)
			{
				temp = i->content;
				i->content = j->content;
				j->content = temp;
			}
			j = j->next;
		}
		i = i->next;
	}
}

//Second export list util
void	print_list(t_list *head)
{
	t_list	*temp;

	temp = head;
	sort_list(head);
	while (temp)
	{
		ft_printf("declare -x %s\n", (char *)temp->content);
		temp = temp->next;
	}
}

int	with_value(char *string)
{
	while (*string)
	{
		if (*(string++) == '=')
			return (1);
	}
	return (0);
}

char	*ft_strcat(char *dest, const char *src)
{
	char	*ptr;

	if (!dest || !src)
		return (NULL);
	ptr = dest;
	while (*ptr != '\0')
		ptr++;
	while (*src != '\0')
	{
		*ptr = *src;
		ptr++;
		src++;
	}
	*ptr = '\0';
	return (dest);
}
