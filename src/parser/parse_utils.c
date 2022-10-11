/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 11:05:28 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/10/06 11:05:31 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	utils_irdouky(t_data *redi, t_env *env, t_env *origin, char *s1)
{
	char	*s2;
	int		i;

	i = 0;
	s2 = readline("irdouk> ");
	if (!s2)
	{
		signal_irdouk(s1);
		close(redi->in);
		free(s2);
		ft_exit(redi, env, origin, 0);
	}
	if (!ft_strcmp(s1, s2))
	{
		close(redi->in);
		free(s2);
		ft_exit(0, env, origin, 0);
	}
	s2 = utils_irdouk(s2, env, 0, 0);
	while (s2[i])
		write(redi->in, &s2[i++], 1);
	write(redi->in, "\n", 1);
	free(s2);
}

char	*utils_irdouk(char *str, t_env *env, int i, int j)
{
	t_input	irdouk;

	if (!str)
		return (0);
	irdouk.s1 = ft_strdup(str);
	irdouk.s2 = malloc(sizeof(char) * get_promptlen(str, env, 1));
	if (!irdouk.s2)
		return (free(str), NULL);
	free(str);
	while (irdouk.s1[i])
	{
		if (irdouk.s1[i] == '$' && irdouk.s1[i + 1]
			&& (ft_strchr(SET, irdouk.s1[i + 1]) || (irdouk.s1[i + 1] == '?')))
		{
			if (irdouk.s1[i + 1] >= '0' && irdouk.s1[i + 1] <= '9')
				i++;
			else
				get_dollar(&irdouk, env, &i, &j);
			i++;
		}
		else
			irdouk.s2[j++] = irdouk.s1[i++];
	}
	return (irdouk.s2[j] = '\0', free(irdouk.s1), irdouk.s2);
}

static void	utils_parse(t_data *data, t_data *tmp)
{
	if (!data || !tmp)
		return ;
	if ((tmp->code == IN || tmp->code == DBIN)
		&& data->in != -1)
	{
		if (data->in > 2)
			close(data->in);
		if (data->path)
		{
			unlink(data->path);
			free(data->path);
		}
		data->in = tmp->in;
		data->path = tmp->path;
	}
	else if (tmp->code == OUT || tmp->code == DBOUT)
	{
		if (data->out > 2)
			close(data->out);
		data->out = tmp->out;
	}
}

void	utils_deltmp(t_data *data, t_data **tmp, t_data **head)
{
	if (!data && (!tmp || !*tmp || !head || !*head))
		return ;
	if (data && data->code != PIPE)
		utils_parse(data, *tmp);
	if ((*tmp)->prev)
	{
		*tmp = (*tmp)->prev;
		if (data && data->code != PIPE)
			data_delelem(tmp, 1, 0);
		else
			data_delelem(tmp, 1, 1);
		*tmp = (*tmp)->next;
	}
	else
	{
		*head = (*head)->next;
		if (data && data->code != PIPE)
			data_delelem(tmp, 0, 0);
		else
			data_delelem(tmp, 0, 1);
	}
}

void	utils_next(t_data **data, t_data **head)
{
	if (!data || !*data || !head || !*head)
		return ;
	if (*data && (*data)->next && (*data)->next->code < PIPE)
		init_args(*data);
	if (*data && (*data)->code == PIPE)
	{
		if ((*data)->prev)
		{
			*data = (*data)->prev;
			data_delelem(data, 1, 1);
			*data = (*data)->next;
		}
		else
		{
			*head = (*head)->next;
			data_delelem(data, 0, 1);
		}
	}
	else if (*data)
		*data = (*data)->next;
}
