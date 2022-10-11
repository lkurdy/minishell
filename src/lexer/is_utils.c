/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 19:09:08 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/24 20:13:44 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_inquotes(char *str, int pos)
{
	int	i;
	int	s;
	int	d;

	i = 0;
	s = 0;
	d = 0;
	while (str && i <= pos)
	{
		if (s == 0 && d == 0 && str[i] == '\'')
			s = 1;
		else if (d == 0 && s == 0 && str[i] == '\"')
			d = 1;
		else if (s == 1 && d == 0 && str[i] == '\'')
			s = 0;
		else if (d == 1 && s == 0 && str[i] == '\"')
			d = 0;
		i++;
	}
	if (s == 0 && d == 1 && str && str[pos] != '\"')
		return (2);
	else if (s == 1 && d == 0 && str && str[pos] != '\'')
		return (1);
	return (0);
}

int	is_token(char *str, int i)
{
	if (str && i < ft_strlen(str))
	{
		if ((str[i + 1] && str[i] == '<' && str[i + 1] == '>')
			|| (str[i + 1] && str[i] == '>' && str[i + 1] == '<'))
			return (1);
		else if (i > 0 && !is_inquotes(str, i - 1)
			&& (str[i - 1] == '<' || str[i - 1] == '>')
			&& (str[i] == '<' || str[i] == '>'))
			return (1);
		else if (str[i + 1] && (str[i] == '<' || str[i] == '>')
			&& str[i + 1] != '|' && str[i + 1] != '<' && str[i + 1] != '>')
			return (1);
		else if (str[i + 1] && !is_inquotes(str, i + 1) && str[i] != '<'
			&& str[i] != '>' && str[i + 1] != '|' && (str[i + 1] == '<'
				|| str[i + 1] == '>'))
			return (1);
		else if (str[i] == '|' || (str[i + 1] && !is_inquotes(str, i + 1)
				&& str[i] != '|' && str[i + 1] && str[i + 1] == '|'))
			return (1);
	}
	return (0);
}

char	*is_dollar(char *str, int i)
{
	char	*dollar;
	int		j;
	int		tmp;

	if (!str)
		return (0);
	tmp = ++i;
	j = 0;
	while (str[tmp] && (ft_strchr(SET, str[tmp])))
	{
		j++;
		tmp++;
	}
	dollar = malloc(sizeof(char) * j + 1);
	if (!dollar)
		return (0);
	j = 0;
	while (str[i] && (ft_strchr(SET, str[i])))
		dollar[j++] = str[i++];
	dollar[j] = '\0';
	return (dollar);
}

char	*is_content(char *line)
{
	char	*content;
	int		i;
	int		j;
	int		tmp;

	i = 0;
	j = 0;
	while (line && line[i] && line[i] != '=')
		i++;
	tmp = ++i;
	while (line && line[i++])
		j++;
	content = malloc(sizeof(char) * j + 1);
	if (!content)
		return (0);
	i = 0;
	while (line && line[tmp])
	{
		content[i] = line[tmp++];
		if (content[i] == '<' || content[i] == '>' || content[i] == '|'
			|| content[i] == '\'' || content[i] == '\"')
			content[i] *= -1;
		i++;
	}
	return (content[i] = '\0', content);
}

int	is_ecode(void)
{
	int	tmp;
	int	i;

	i = 1;
	tmp = g_ecode;
	while (tmp > 9)
	{
		tmp /= 10;
		i++;
	}
	return (i);
}
