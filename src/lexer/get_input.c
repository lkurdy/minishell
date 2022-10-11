/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 21:48:52 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/01 23:55:51 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_ecode(char *prompt, int i)
{
	int		tmp;
	int		j;
	char	swp;

	if (!prompt)
		return (0);
	tmp = g_ecode;
	j = i;
	while (tmp > 9)
	{
		prompt[i++] = (tmp % 10) + '0';
		tmp /= 10;
	}
	prompt[i] = tmp + '0';
	while (j <= i)
	{
		swp = prompt[i];
		prompt[i] = prompt[j];
		prompt[j++] = swp;
		i--;
	}
	return (prompt);
}

static void	get_contlen(t_env *env, char *str, int *i, int *j)
{
	char	*dollar;
	char	*content;

	if (!str)
		return ;
	if (str[*i + 1] == '?')
	{
		*j += is_ecode();
		*i += 2;
		return ;
	}
	if (str[*i + 1] >= '0' && str[*i + 1] <= '9')
		*i += 2;
	else
	{
		dollar = is_dollar(str, *i);
		content = check_dollar(env, str, *i);
		*j += ft_strlen(content) + 1;
		*j -= ft_strlen(dollar);
		*i += ft_strlen(dollar) + 1;
		free(dollar);
		free(content);
	}
}

int	get_promptlen(char *str, t_env *env, int val)
{
	int	i;
	int	j;

	if (!str)
		return (0);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (!val && str[i] == '$' && is_inquotes(str, i) != 1 && str[i + 1]
			&& (ft_strchr(SET, str[i + 1]) || (str[i + 1] == '?')))
			get_contlen(env, str, &i, &j);
		else if (val && str[i] == '$' && str[i + 1]
			&& (ft_strchr(SET, str[i + 1]) || (str[i + 1] == '?')))
			get_contlen(env, str, &i, &j);
		else
		{
			if (!val && !is_inquotes(str, i) && is_token(str, i))
				j++;
			i++;
		}
	}
	return (i + j + 1);
}

void	get_dollar(t_input *input, t_env *env, int *i, int *j)
{
	char	*content;
	int		k;

	if (!input | !input->s1 || !input->s2)
		return ;
	if (input->s1[*i + 1] == '?')
	{
		input->s2 = get_ecode(input->s2, *j);
		*j += is_ecode();
		*i += 1;
		return ;
	}
	content = check_dollar(env, input->s1, *i);
	if (content)
	{
		k = 0;
		while (content[k])
		{
			input->s2[*j] = content[k++];
			*j += 1;
		}
		free(content);
	}
	while (input->s1[*i + 1] && (ft_strchr(SET, input->s1[(*i + 1)])))
		*i += 1;
}

int	get_input(t_input *input, t_env *env, int i, int j)
{
	while (input && input->s1 && input->s2 && input->s1[i])
	{
		if (input->s1[i] == '$' && is_inquotes(input->s1, i) != 1
			&& input->s1[i + 1] && (ft_strchr(SET, input->s1[i + 1])
				|| (input->s1[i + 1] == '?')))
		{
			if (input->s1[i + 1] >= '0' && input->s1[i + 1] <= '9')
				i++;
			else
			{
				get_dollar(input, env, &i, &j);
				if (!is_inquotes(input->s1, i) && is_token(input->s1, i))
					input->s2[j++] = ' ';
			}
			i++;
		}
		else if (!is_inquotes(input->s1, i) && is_token(input->s1, i))
		{
			input->s2[j++] = input->s1[i++];
			input->s2[j++] = ' ';
		}
		else
			input->s2[j++] = input->s1[i++];
	}
	return (j);
}
