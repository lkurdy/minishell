/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkurdy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:45 by lkurdy            #+#    #+#             */
/*   Updated: 2022/07/27 14:23:25 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

static int	size(const char *str, char set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (((str[i] != set) && (str[i + 1] == set))
			|| ((str[i] != set) && !str[i + 1]))
			j++;
		i++;
	}
	return (j);
}

static char	*ft_cpy(char *dest, const char *str, int n)
{
	int	i;

	i = 0;
	while (str[i] && i < n)
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	**remp(int *i, int *j, const char *s, char c)
{
	char	**dest;

	if (!s)
		return (NULL);
	dest = malloc(sizeof(const char *) * (size(s, c) + 1));
	*i = 0;
	*j = 0;
	return (dest);
}

char	**ft_split(const char *s, char c)
{
	char	**dest;
	int		i;
	int		j;
	int		g;

	dest = remp(&i, &j, s, c);
	if (!dest)
		return (NULL);
	while (j < size(s, c) && s[i])
	{
		g = i;
		while ((s[i] != c) && s[i])
			i++;
		if (i)
		{
			dest[j] = malloc(sizeof(const char) * (i - g + 1));
			if (!dest[j])
				return (ft_free(dest, 0), NULL);
			dest[j] = ft_cpy(dest[j], &s[g], i - g);
			j++;
		}
		while ((s[i] == c) && s[i])
			i++;
	}
	return (dest[j] = 0, dest);
}
