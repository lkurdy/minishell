/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 18:06:49 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/26 15:23:05 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	data_newback(t_data **data, char *str)
{
	t_data	*tmp;
	t_data	*new;

	new = malloc(sizeof(t_data));
	if (!new)
		return (0);
	new->code = 0;
	new->cmd = ft_strdup(str);
	new->args = 0;
	new->in = 0;
	new->out = 1;
	new->a = 0;
	new->b = 0;
	new->exit = 0;
	new->path = 0;
	new->next = 0;
	new->prev = 0;
	if (!data || !*data)
		return (*data = new, 0);
	tmp = *data;
	while (tmp->next)
		tmp = tmp->next;
	new->prev = tmp;
	return (tmp->next = new, 0);
}

t_data	*data_create(char *prompt)
{
	char	**tab;
	t_data	*data;
	t_data	*tmp;
	int		i;

	tab = split_input(prompt);
	if (!tab && g_ecode == -42)
		return (NULL);
	else if (!tab)
		return (g_ecode = 2, NULL);
	data = 0;
	i = 0;
	while (tab[i])
		data_newback(&data, tab[i++]);
	ft_free(tab, 0);
	tmp = data;
	while (tmp)
	{
		tmp->code = check_code(tmp->cmd);
		if (tmp->prev && tmp->prev->code >= PIPE && tmp->code == PIPE)
			return (g_ecode = 2, ft_free(0, &data), NULL);
		tmp = tmp->next;
	}
	return (data);
}

static void	data_delutils(t_data *data, int val)
{
	if (!data)
		return ;
	free(data->cmd);
	data->cmd = 0;
	ft_free(data->args, 0);
	data->args = 0;
	if (val)
	{
		if (data->in > 2)
			close(data->in);
		if (data->out > 2)
			close(data->out);
		if (data->a > 2)
			close(data->a);
		if (data->b > 2)
			close(data->b);
		if (data->path)
		{
			unlink(data->path);
			free(data->path);
			data->path = 0;
		}
	}
	free(data);
	data = 0;
}

void	data_delelem(t_data **data, int i, int j)
{
	t_data	*tmp;

	if (!data || !*data)
		return ;
	if (!i)
	{
		if (!*data)
			return ;
		tmp = *data;
		*data = (*data)->next;
		if (*data)
			(*data)->prev = 0;
	}
	else
	{
		if (!(*data)->next)
			return ;
		tmp = (*data)->next;
		(*data)->next = tmp->next;
		if (tmp->next)
			tmp->next->prev = *data;
	}
	data_delutils(tmp, j);
}

void	data_init(t_data *data, t_env *env, t_env *origin)
{
	t_data	*tmp;
	int		i;

	tmp = data;
	while (tmp)
	{
		i = 0;
		if (tmp->next && tmp->code > PIPE)
			init_redi(tmp, env, origin);
		while (tmp->cmd[i])
		{
			if (tmp->cmd[i] == - '<' || tmp->cmd[i] == - '>'
				|| tmp->cmd[i] == - '|' || tmp->cmd[i] == - '\''
				|| tmp->cmd[i] == - '\"')
				tmp->cmd[i] *= -1;
			i++;
		}
		tmp = tmp->next;
	}
}
