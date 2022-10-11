/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 01:45:19 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/21 19:26:10 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *str)
{
	int	i;
	int	s;
	int	d;

	i = 0;
	s = 0;
	d = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' && d == 0)
			s++;
		else if (str[i] == '\"' && s == 0)
			d++;
		if (s == 2)
			s = 0;
		else if (d == 2)
			d = 0;
		i++;
	}
	if (s % 2 == 1)
		return (1);
	if (d % 2 == 1)
		return (2);
	return (0);
}

static t_data	*check_redi(t_data *data)
{
	t_data	*tmp;

	tmp = data;
	while (tmp)
	{
		if (tmp->code > PIPE && (!tmp->next || tmp->next->code > PIPE))
		{
			if (tmp->next)
			{
				write(2, "minihlel: syntax error near unexpected token `", 46);
				write(2, &tmp->cmd[0], 1);
				if (tmp->cmd[1])
					write(2, &tmp->cmd[1], 1);
				write(2, "'\n", 2);
			}
			else
			{
				write(2, "minihlel: syntax error ", 23);
				write(2, "near unexpected token `newline'\n", 32);
			}
			return (g_ecode = 2, ft_free(0, &data), NULL);
		}
		tmp = tmp->next;
	}
	return (data);
}

t_codes	check_code(char *str)
{
	if (str && !ft_strcmp(str, "echo"))
		return (ECH);
	else if (str && !ft_strcmp(str, "cd"))
		return (CD);
	else if (str && !ft_strcmp(str, "pwd"))
		return (PWD);
	else if (str && !ft_strcmp(str, "export"))
		return (EXPORT);
	else if (str && !ft_strcmp(str, "unset"))
		return (UNSET);
	else if (str && !ft_strcmp(str, "env"))
		return (ENV);
	else if (str && !ft_strcmp(str, "exit"))
		return (EXIT);
	else if (str && !ft_strcmp(str, "<"))
		return (IN);
	else if (str && !ft_strcmp(str, ">"))
		return (OUT);
	else if (str && !ft_strcmp(str, "<<"))
		return (DBIN);
	else if (str && !ft_strcmp(str, ">>"))
		return (DBOUT);
	else if (str && !ft_strcmp(str, "|"))
		return (PIPE);
	return (0);
}

char	*check_dollar(t_env *env, char *str, int i)
{
	char	*dollar;
	char	*content;

	if (!env || !str)
		return (0);
	dollar = is_dollar(str, i);
	while (env && env->line)
	{
		if (!ft_strncmp(dollar, env->line, ft_strlen(dollar))
			&& env->line[ft_strlen(dollar)] == '=')
		{
			content = is_content(env->line);
			return (free(dollar), content);
		}
		env = env->next;
	}
	return (free(dollar), NULL);
}

t_data	*check_errors(t_input *input, t_env *env)
{
	t_data	*data;

	if (!input || !input->s1)
		return (0);
	if (check_quotes(input->s1))
	{
		write(2, "minihlel: syntax error ", 23);
		if (check_quotes(input->s1) == 1)
			write(2, "near unexpected simple quote\n", 29);
		else
			write(2, "near unexpected double quote\n", 29);
		g_ecode = 2;
		return (free(input->s1), NULL);
	}
	input->s2 = malloc(sizeof(char) * get_promptlen(input->s1, env, 0));
	if (!input->s2)
		return (free(input->s1), NULL);
	input->s2[get_input(input, env, 0, 0)] = '\0';
	data = data_create(input->s2);
	if (!data && g_ecode == 2)
		write(2, "minihlel: syntax error near unexpected token `|'\n", 49);
	else if (!data && g_ecode == -42)
		g_ecode = 0;
	return (free(input->s1), check_redi(data));
}
