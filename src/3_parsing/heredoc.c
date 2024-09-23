/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 12:06:11 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	*heredoc_child(char *delimiter, t_data *data);
static char	combine_lines(char **out, char *new_line);

// for << operator with child process
int	heredoc(char *delimiter, t_data *data)
{
	int		fd[2];
	pid_t	id;
	char	*line;

	if (pipe(fd) == -1)
		return (1);
	id = fork();
	if (id == -1)
		return (1);
	if (id == 0)
	{
		handle_signals(3);
		close(fd[0]);
		line = heredoc_child(delimiter, data);
		if (line)
			write(fd[1], line, ft_strlen(line));
		free(line);
		close(fd[1]);
		exit(ft_free(data, 0));
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(id, &data->status, 0);
	return (WEXITSTATUS(data->status));
}

static char	*heredoc_child(char *delimiter, t_data *data)
{
	char	*new_line;
	char	*out;

	if (!restore_stdin_stdout(data, 2))
		exit (ft_free(data, 1));
	out = ft_strdup("");
	if (!out)
		return (NULL);
	while (1)
	{
		new_line = readline("> ");
		if (!new_line)
		{
			error_message("warning",
				"here-document delimited by end-of-file - wanted", delimiter);
			break ;
		}
		if (!ft_strncmp(new_line, delimiter, ft_strlen(delimiter) + 1))
			return (free(new_line), out);
		if (!combine_lines(&out, new_line))
			return (NULL);
	}
	return (out);
}

static char	combine_lines(char **out, char *new_line)
{
	char	*tmp;

	tmp = ft_strjoin(*out, new_line);
	free(*out);
	free(new_line);
	if (!tmp)
		return (0);
	*out = tmp;
	tmp = ft_strjoin(*out, "\n");
	free(*out);
	if (!tmp)
		return (0);
	*out = tmp;
	return (1);
}
