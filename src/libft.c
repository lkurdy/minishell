/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 21:02:19 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/08/31 22:14:41 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if ((char)s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)s[i] == (char)c)
		return ((char *)&s[i]);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (1);
	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	if (!s1 || !s2)
		return (1);
	i = 0;
	if (!n)
		return (0);
	while (s1[i] && s2[i] && i < n - 1 && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strdup(char *s)
{
	int		i;
	int		y;
	char	*dest;

	if (!s)
		return (0);
	i = 0;
	y = 0;
	while (s[y] != '\0')
		y++;
	dest = malloc(sizeof(char) * y + 1);
	if (!dest)
		return (0);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*c;
	int		i;
	int		y;

	if (!s1)
		return (0);
	i = 0;
	y = 0;
	while (s1[i])
		i++;
	while (s2 && s2[y])
		y++;
	c = malloc(sizeof(char) * (i + y + 1));
	if (!c)
		return (NULL);
	i = 0;
	y = 0;
	while (s1[i])
	{
		c[i] = s1[i];
		i++;
	}
	while (s2 && s2[y])
		c[i++] = s2[y++];
	return (c[i] = '\0', c);
}
