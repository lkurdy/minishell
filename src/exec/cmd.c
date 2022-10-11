/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 16:38:43 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/27 16:38:49 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exe(char **o, char **envp)
{
	char	**paths;
	int		j;
	char	*bin;

	j = 0;
	paths = NULL;
	if (!envp || !envp[0] || ft_strchr(o[0], '/'))
		exe_remp(o, envp);
	else
	{
		paths = ft_parse(envp);
		while (paths && paths[j])
		{
			bin = ft_strjoin(paths[j], o[0]);
			if (!bin)
				break ;
			execve(bin, o, envp);
			free(bin);
			j++;
		}
		write(2, o[0], ft_strlen(o[0]));
		write(2, ": command not found\n", 20);
	}
	return (ft_free(paths, 0), 127);
}

static int	pipex(t_data *tmp, t_env *mini, t_data *data, t_env *origin)
{
	pid_t	process;
	int		fd[2];
	char	**envp;
	char	**cmd;

	cmd = fuse(tmp->cmd, tmp->args);
	envp = ft_envp(mini);
	pipe(fd);
	process = fork();
	if (process < 0)
		return (0);
	if (!process)
	{
		pipex_remp(fd, tmp);
		if (tmp->code)
			g_ecode = cmd_parse(tmp, mini, origin);
		else
			exe(cmd, envp);
		ft_free(envp, 0);
		ft_free(cmd, 0);
		return (close(fd[1]), ft_exit(data, mini, origin, 0), 0);
	}
	ft_free(envp, 0);
	return (ft_free(cmd, 0), close(fd[1]), dup2(fd[0], 0), close(fd[0]), 0);
}

static int	last_exe(t_data *tmp, t_env *mini, t_data *data, t_env *origin)
{
	pid_t	process;
	char	**envp;
	char	**cmd;
	int		status;

	cmd = fuse(tmp->cmd, tmp->args);
	envp = ft_envp(mini);
	process = fork();
	if (process < 0)
		return (0);
	signal(SIGQUIT, signal_quit);
	if (!process)
	{
		signal(SIGINT, SIG_DFL);
		if (!tmp->code)
			g_ecode = exe(cmd, envp);
		else
			g_ecode = cmd_parse(tmp, mini, origin);
		ft_free(envp, 0);
		return (ft_free(cmd, 0), ft_exit(data, mini, origin, 0), 0);
	}
	waitpid(process, &status, 0);
	if (WIFEXITED(status))
		g_ecode = WEXITSTATUS(status);
	return (signal(SIGQUIT, SIG_IGN), ft_free(cmd, 0), ft_free(envp, 0), 0);
}

static void	loop_remp(t_data *tmp, t_env *mini, t_env *origin, t_data *data)
{
	int	j;

	j = -1;
	if (tmp->out > 1)
		dup2(tmp->out, STDOUT_FILENO);
	if (tmp->in > 0)
		dup2(tmp->in, STDIN_FILENO);
	if (!tmp->prev && !tmp->next)
	{
		tmp->exit = 1;
		g_ecode = cmd_parse(tmp, mini, origin);
		if (g_ecode > -1)
			j = 1;
		tmp->exit = 0;
	}
	if (j < 0)
	{
		g_ecode = -42;
		if (tmp->next)
			pipex(tmp, mini, data, origin);
		else
			last_exe(tmp, mini, data, origin);
	}
}

void	exec_loop(t_env *mini, t_env *origin, t_data *data)
{
	t_data	*tmp;
	int		out;
	int		in;

	in = dup(0);
	out = dup(1);
	data->a = in;
	data->b = out;
	tmp = data;
	while (tmp)
	{
		if (tmp->in > -1 && tmp->out > -1)
			loop_remp(tmp, mini, origin, data);
		if (tmp->out > 1)
			dup2(out, 1);
		tmp = tmp->next;
		if (tmp && tmp->in > 0)
			dup2(in, 0);
	}
	clean_dup(out, in);
	while (wait(NULL) > 0)
		;
}
