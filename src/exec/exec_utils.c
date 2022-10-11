/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 18:34:07 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/10/03 18:34:10 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_parse(char **envp)
{
	char	*s;
	int		i;
	char	**paths;

	i = 0;
	while (envp && envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp || !envp[i])
		return (NULL);
	s = &envp[i][5];
	paths = ft_splity(s, ':');
	return (paths);
}

void	exe_remp(char **o, char **envp)
{
	execve(o[0], o, envp);
	write(2, "minihlel: ", 10);
	perror(o[0]);
}

void	clean_dup(int out, int in)
{
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(out);
	close(in);
}

void	pipex_remp(int *fd, t_data *tmp)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	close(fd[0]);
	if (tmp->out == 1)
		dup2(fd[1], 1);
	else
		close(fd[1]);
}

void	echo_remp(char *str, int opt, int i)
{
	if (str && str[i])
		write(1, &str[i], ft_strlen(&str[i]));
	if (!opt)
		write(1, "\n", 1);
	if (str)
		free(str);
}
