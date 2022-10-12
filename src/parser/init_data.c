/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 13:02:38 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/26 15:24:30 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_file(t_data *redi, int i)
{
	char	*str;
	int		j;

	if (!redi)
		return ;
	while (1)
	{
		j = 1;
		str = malloc(sizeof(char) * (i + 2));
		if (!str)
			return ;
		str[0] = '.';
		while (j - 1 < i)
			str[j++] = 'x';
		str[j] = '\0';
		redi->path = ft_strjoin("/tmp/", str);
		redi->in = open(redi->path, O_CREAT | O_RDWR | O_TRUNC, 0664);
		free(str);
		if (redi->in > 2)
			return ;
		close(redi->in);
		free(redi->path);
		i++;
	}
}

static int	init_irdouk(t_data *redi, char *s1, t_env *env, t_env *origin)
{
	pid_t	process;
	int		status;

	init_file(redi, 1);
	process = fork();
	if (process < 0)
		return (close(redi->in), -1);
	g_ecode = 0;
	while (!process && s1 && redi)
	{
		signal(SIGINT, SIG_DFL);
		utils_irdouky(redi, env, origin, s1);
	}
	if (!process)
		ft_exit(0, env, origin, 0);
	waitpid(process, &status, 0);
	if (WIFEXITED(status))
		return (close(redi->in), open(redi->path, O_RDWR));
	return (close(redi->in), -1);
}

void	init_redi(t_data *redi, t_env *env, t_env *origin)
{
	if (redi && redi->code == IN && redi->next->code < PIPE)
		redi->in = open(redi->next->cmd, O_RDWR);
	else if (redi && redi->code == OUT && redi->next->code < PIPE)
		redi->out = open(redi->next->cmd, O_CREAT | O_RDWR | O_TRUNC, 0664);
	else if (redi && redi->code == DBIN && redi->next->code < PIPE)
		redi->in = init_irdouk(redi, redi->next->cmd, env, origin);
	else if (redi && redi->code == DBOUT && redi->next->code < PIPE)
		redi->out = open(redi->next->cmd, O_CREAT | O_RDWR | O_APPEND, 0664);
	if (redi && redi->in == -1 && g_ecode != 0 && g_ecode != 130)
	{
		write(2, "minihlel: ", 10);
		perror(redi->next->cmd);
		g_ecode = 1;
	}
	else if (redi && redi->out == -1)
	{
		write(2, "minihlel: ", 10);
		perror(redi->next->cmd);
		g_ecode = 1;
	}
	data_delelem(&redi, 1, 1);
}

void	init_args(t_data *data)
{
	t_data	*tmp;
	int		i;

	if (!data || data->code == PIPE || !data->next)
		return ;
	tmp = data;
	i = 0;
	while (tmp && tmp->code != PIPE)
	{
		tmp = tmp->next;
		i++;
	}
	data->args = malloc(sizeof(char *) * i);
	if (!data)
		return ;
	i = 0;
	while (data && data->next && data->next->code != PIPE)
	{
		data->args[i++] = ft_strdup(data->next->cmd);
		data_delelem(&data, 1, 1);
	}
	data->args[i] = 0;
}

void	init_cmdset(t_data **data, t_env *env, t_env *origin)
{
	t_data	*head;
	t_data	*tmp;

	if (!data || !*data)
		return ;
	data_init(*data, env, origin);
	head = *data;
	while (*data)
	{
		tmp = *data;
		while (*data && (*data)->code > PIPE)
			*data = (*data)->next;
		while (tmp && tmp->code != PIPE)
		{
			if (tmp->code > PIPE)
				utils_deltmp(*data, &tmp, &head);
			else
				tmp = tmp->next;
		}
		utils_next(data, &head);
	}
	*data = head;
}
