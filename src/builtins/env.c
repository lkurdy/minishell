/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:43:09 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/10/11 23:28:48 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	joseph(char **args)
{
	int	i;
	int	j;
	int	d;
	int	y;

	i = -1;
	while (args[++i])
	{
		j = 0;
		while (args[i][j])
			j++;
		d = ft_atoi(args[i], &y);
		if (y < j)
		{
			write(2, "minihlel: exit: ", 16);
			if (!i)
				return (write(2, "numeric argument required\n", 26), 2);
			else
				return (write(2, "too many arguments\n", 19), -67231);
		}
		if (i > 0)
			return (write(2, "minihlel: exit: too many arguments\n", 35), -67231);
	}
	return (d);
}

void	ft_exit(t_data *data, t_env *mini, t_env *origin, char **args)
{
	int	a;

	a = 0;
	if (args)
	{
		a = joseph(args);
		if (a == -67231)
			return ;
		g_ecode = a;
	}
	if (data && data->exit)
		printf("exit\n");
	lst_freeall(mini);
	lst_freeall(origin);
	ft_free(0, &data);
	exit(g_ecode);
}

t_env	*ft_env(char **env)
{
	t_env	*mini_env;
	int		i;

	i = 1;
	mini_env = malloc(sizeof(t_env));
	if (!mini_env)
		return (NULL);
	if (!env[0])
		return (mini_env->line = NULL, mini_env->next = NULL, mini_env);
	mini_env->line = ft_strdup(env[0]);
	if (!mini_env->line)
		return (free(mini_env), NULL);
	mini_env->next = NULL;
	while (env[i])
	{
		if (!lst_addback(mini_env, env[i]))
			return (lst_freeall(mini_env), NULL);
		i++;
	}
	shlvl(mini_env);
	return (mini_env);
}

void	ft_printenv(t_env *mini_env)
{
	t_env	*temp;

	temp = mini_env;
	while (temp && temp->line)
	{
		printf("%s\n", temp->line);
		temp = temp->next;
	}
}

void	ft_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, 1024) == NULL)
		return ;
	printf("%s\n", buf);
}
