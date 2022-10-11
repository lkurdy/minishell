/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkurdy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 14:19:25 by lkurdy            #+#    #+#             */
/*   Updated: 2022/09/30 12:58:55 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lst_modone(t_env *mini_env, char *str, char *new)
{
	t_env	*temp;
	int		i;

	i = 0;
	while (str && str[i])
		i++;
	temp = mini_env;
	while (temp && temp->line)
	{
		if (!ft_strncmp(temp->line, str, i))
		{
			free(temp->line);
			temp->line = new;
			return ;
		}
		temp = temp->next;
	}
}

void	lst_freeall(t_env *mini_env)
{
	t_env	*temp;

	while (mini_env)
	{
		temp = mini_env;
		mini_env = mini_env->next;
		if (temp->line)
			free(temp->line);
		free(temp);
	}
}

void	lst_delone(t_env *mini_env, char *env)
{
	t_env	*pre_temp;
	t_env	*temp;
	int		d;

	d = 0;
	pre_temp = mini_env;
	while (mini_env && mini_env->line)
	{
		if (!ft_strncmp(mini_env->line, env, ft_strlen(env)))
		{
			temp = mini_env->next;
			pre_temp->next = temp;
			free(mini_env->line);
			if (d)
				free(mini_env);
			else
				mini_env->line = NULL;
			return ;
		}
		pre_temp = mini_env;
		mini_env = mini_env->next;
		d++;
	}
}

int	lst_addback(t_env *mini_env, char *env)
{
	t_env	*temp;
	t_env	*env_clone;

	env_clone = mini_env;
	if (!env_clone->line)
		return (env_clone->line = ft_strdup(env), 1);
	temp = malloc(sizeof(t_env));
	if (!temp)
		return (0);
	temp->line = ft_strdup(env);
	temp->next = NULL;
	if (!temp->line)
		return (free(temp), 0);
	while (env_clone->next)
		env_clone = env_clone->next;
	env_clone->next = temp;
	return (1);
}

char	**ft_envp(t_env *mini)
{
	char	**env;
	int		i;
	t_env	*temp;

	if (!mini->line)
		return (NULL);
	temp = mini;
	i = 0;
	while (mini)
	{
		mini = mini->next;
		i++;
	}
	env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	if (!env)
		return (NULL);
	while (temp)
	{
		env[i] = ft_strdup(temp->line);
		i++;
		temp = temp->next;
	}
	return (env[i] = 0, env);
}
