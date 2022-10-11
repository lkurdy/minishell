/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:02:19 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/14 19:33:54 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_tablen(char *str)
{
	int	i;
	int	j;

	if (!str)
		return (0);
	i = 0;
	j = 1;
	while (str[i] && !is_inquotes(str, i) && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!is_inquotes(str, i) && (str[i] == ' ' || str[i] == '\t')
			&& str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '\t')
			j++;
		i++;
	}
	return (j);
}

static int	ft_stablen(char *str, int tor)
{
	static int	i;
	int			j;

	if (!str)
		return (0);
	if (tor == 0)
		i = 0;
	j = 0;
	while (str[i] && !is_inquotes(str, i) && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i] && ((str[i] != ' ' && str[i] != '\t') || is_inquotes(str, i)))
	{
		if (!is_inquotes(str, i) && (str[i] == '\'' || str[i] == '\"'))
			j--;
		i++;
		j++;
	}
	return (j);
}

static char	*ft_strtab(char *str, int tor)
{
	char		*strtab;
	static int	i;
	int			j;

	if (!str)
		return (0);
	if (tor == 0)
		i = 0;
	j = 0;
	strtab = malloc(sizeof(char) * ft_stablen(str, tor) + 1);
	if (!strtab)
		return (0);
	while (str[i] && !is_inquotes(str, i) && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i] && ((str[i] != ' ' && str[i] != '\t') || is_inquotes(str, i)))
	{
		if (!is_inquotes(str, i) && (str[i] == '\'' || str[i] == '\"'))
			i++;
		else
			strtab[j++] = str[i++];
	}
	strtab[j] = '\0';
	return (strtab);
}

char	**split_input(char *str)
{
	char	**tab;
	int		count;
	int		i;

	count = ft_tablen(str);
	if (count == 0)
		return (g_ecode = -42, free(str), NULL);
	tab = malloc(sizeof(char *) * (count + 1));
	if (!tab)
		return (free(str), NULL);
	i = 0;
	while (count--)
	{
		tab[i] = ft_strtab(str, i);
		i++;
	}
	tab[i] = 0;
	count = ft_tablen(str);
	if (tab[0] && (tab[0][0] == '|' || (tab[count - 1][0] == '|'
		&& !tab[count - 1][1])))
		return (ft_free(tab, 0), free(str), NULL);
	return (free(str), tab);
}

void	ft_free(char **dest, t_data **data)
{
	int	i;

	if (dest)
	{
		i = 0;
		while (dest[i])
			i++;
		while (i >= 0)
		{
			free(dest[i]);
			dest[i--] = 0;
		}
		free(dest);
		dest = 0;
	}
	else if (data)
	{
		while (*data && (*data)->prev)
			*data = (*data)->prev;
		while (*data)
			data_delelem(data, 0, 1);
		data = 0;
	}
}
