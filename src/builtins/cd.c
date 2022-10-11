/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkurdy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 14:26:14 by lkurdy            #+#    #+#             */
/*   Updated: 2022/09/30 13:51:32 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_home(t_env *temp)
{
	int		i;
	int		j;
	char	*home;

	i = 5;
	j = 0;
	while (temp)
	{
		if (!ft_strncmp(temp->line, "HOME", 4))
		{
			while (temp->line[i])
				i++;
			home = malloc(sizeof(char) * i - 4);
			if (!home)
				return (NULL);
			i = 5;
			while (temp->line[i])
				home[j++] = temp->line[i++];
			return (home[j] = '\0', home);
		}
		temp = temp->next;
	}
	return (NULL);
}

static int	ft_cd2(t_env *mini_env)
{
	char	*new;
	int		cd_return;

	new = get_home(mini_env);
	if (!new)
	{
		write(2, "minihlel: cd: HOME not set\n", 27);
		return (1);
	}
	cd_return = chdir(new);
	free(new);
	return (cd_return);
}

static int	cd_remp(char *args, t_env *mini_env, t_env *origin, char *old)
{
	int	cd_return;

	lst_modone(mini_env, "OLDPWD=", old);
	lst_modone(origin, "OLDPWD=", ft_strdup(old));
	if (!args)
		cd_return = ft_cd2(mini_env);
	else
	{
		cd_return = chdir(args);
		if (cd_return < 0)
		{
			write(2, "minihlel: cd: ", 14);
			perror(args);
			return (1);
		}
	}
	return (cd_return);
}

static int	ft_cd(char *args, t_env *mini_env, t_env *origin)
{
	int		cd_return;
	char	*old;
	char	buf[1024];
	char	*new;

	if (!mini_env->line)
		return (write(2, "env: ‘cd’: No such file or directory\n", 42), 127);
	if (getcwd(buf, 1024) == NULL)
		return (ft_cd2(mini_env));
	old = ft_strjoin("OLDPWD=", buf);
	if (!old)
		return (1);
	cd_return = cd_remp(args, mini_env, origin, old);
	if (cd_return == 1)
		return (1);
	if (getcwd(buf, 1024) == NULL)
		return (1);
	new = ft_strjoin("PWD=", buf);
	if (!new)
		return (1);
	return (lst_modone(mini_env, "PWD", new),
		lst_modone(origin, "PWD", ft_strdup(new)), cd_return);
}

int	pre_cd(t_data *data, t_env *mini, t_env *origin)
{
	if (data->args && data->args[1])
		return (write(2, "minihlel: cd: too many arguments\n", 33), 1);
	else if (data->args)
		return (ft_cd(data->args[0], mini, origin));
	else
		return (ft_cd(NULL, mini, origin));
}
