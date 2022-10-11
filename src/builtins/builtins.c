/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:42:54 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/30 12:59:35 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_strcat(char *dest, char *str, int a, int b)
{
	int	i;
	int	j;

	if (!dest)
		return ;
	i = ft_strlen(dest);
	j = 0;
	while (str && str[j])
	{
		dest[i] = str[j];
		i++;
		j++;
	}
	b--;
	if (a != b)
		dest[i++] = ' ';
	dest[i] = '\0';
}

static int	ft_exp(t_data *data, t_env *mini, t_env *origin)
{
	int	i;
	int	r;

	r = 0;
	i = 0;
	if (data->args && data->args[0])
	{
		while (data->args[i])
		{
			if (lst_exportarg(mini, origin, data->args[i]))
				r = 1;
			i++;
		}
	}
	else if (!origin->line)
		return (write(2, "env: ‘export’: No such file or directory\n", 45), 127);
	else
	{
		lst_sort(origin);
		lst_export(origin);
	}
	return (r);
}

static int	unset(t_env *mini, t_env *origin, t_data *data)
{
	int	i;

	i = 0;
	if (!origin->line)
		return (write(2, "env: ‘unset’: No such file or directory\n", 44), 127);
	while (data->args && data->args[i])
	{
		lst_delone(mini, data->args[i]);
		lst_delone(origin, data->args[i]);
		i++;
	}
	return (0);
}

static char	*ft_piscine(char **strs)
{
	int		i;
	int		j;
	char	*c;

	i = 0;
	j = 0;
	if (!strs || !strs[0])
		return (NULL);
	while (strs[i])
		j = j + ft_strlen(strs[i++]);
	if (i > 0)
		j = j + 1 * (i - 1);
	c = malloc(sizeof(char) * (j + 1));
	if (!c)
		return (NULL);
	c[0] = '\0';
	j = 0;
	while (strs[j])
	{
		ft_strcat(c, strs[j], j, i);
		j++;
	}
	return (c);
}

int	cmd_parse(t_data *data, t_env *mini, t_env *origin)
{
	if (data->code == ECH)
		return (ft_echo(ft_piscine(data->args)), 0);
	if (data->code == CD)
		return (pre_cd(data, mini, origin));
	if (data->code == PWD)
		return (ft_pwd(), 0);
	if (data->code == EXPORT)
		return (ft_exp(data, mini, origin));
	if (data->code == UNSET)
		return (unset(mini, origin, data));
	if (data->code == ENV)
	{
		if (data->args && data->args[0])
			return (write(2, "minihlel: env: too many arguments\n", 34), 1);
		else
			return (ft_printenv(mini), 0);
	}
	if (data->code == EXIT)
		return (ft_exit(data, mini, origin, data->args), 0);
	return (-1);
}
