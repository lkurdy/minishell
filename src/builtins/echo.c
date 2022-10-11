/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkurdy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 14:27:07 by lkurdy            #+#    #+#             */
/*   Updated: 2022/09/30 13:58:05 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*joiny_remp(char *s2, int y, char *c, int i)
{
	int	a;

	a = 1;
	while (s2 && s2[y])
	{
		c[i] = s2[y];
		if (a && s2[y] == '=')
		{
			c[++i] = '"';
			a = 0;
		}
		i++;
		y++;
	}
	if (!a)
		c[i++] = '"';
	if (a)
	{
		c[i++] = '\0';
		c[i++] = '\0';
	}
	return (c[i] = '\0', c);
}

char	*ft_strjoiny(char *s1, char *s2)
{
	char	*c;
	int		i;
	int		y;

	i = 0;
	y = 0;
	c = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 3));
	if (!c)
		return (free(s2), NULL);
	while (s1 && s1[i])
	{
		c[i] = s1[i];
		i++;
	}
	c = joiny_remp(s2, y, c, i);
	return (free(s2), c);
}

char	*rev_strjoiny(char *s1, char *s2)
{
	char	*c;
	int		i;
	int		y;

	c = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!c)
		return (NULL);
	i = 0;
	y = 0;
	while (s1 && s1[i + 1])
	{
		c[i] = s1[i];
		i++;
	}
	if (s1 && s1[i] != '"')
	{
		c[i] = s1[i];
		i++;
	}
	while (s2 && s2[y])
		c[i++] = s2[y++];
	if (i < (ft_strlen(s1) + ft_strlen(s2)))
		c[i++] = '"';
	return (c[i] = '\0', c);
}

char	*ft_strdupy(char *s)
{
	int		i;
	int		y;
	char	*dest;

	i = 0;
	y = 0;
	while (s && s[y])
		y++;
	dest = malloc(sizeof(char) * y);
	if (!dest)
		return (0);
	y = 0;
	while (s && s[i])
	{
		if (s[i] != '+')
			dest[y++] = s[i];
		i++;
	}
	dest[y] = '\0';
	return (dest);
}

void	ft_echo(char *str)
{
	int	i;
	int	y;
	int	opt;

	i = 0;
	opt = 0;
	while (str && str[i] && str[i] == '-')
	{
		y = i;
		i++;
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i] || str[i++] == ' ')
			opt++;
		else
		{
			i = y;
			break ;
		}
	}
	echo_remp(str, opt, i);
}
