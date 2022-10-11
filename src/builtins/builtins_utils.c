/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 07:10:44 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/10/08 07:10:45 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	shlvl(t_env *mini)
{
	int	i;

	while (mini)
	{
		if (!ft_strncmp(mini->line, "SHLVL=", 6))
		{
			i = ft_strlen(mini->line);
			mini->line[i - 1]++;
			break ;
		}
		mini = mini->next;
	}
}

void	relou_remp(t_env *origin, char *str, int i, char *tmp)
{
	t_env	*copy;

	copy = origin;
	while (copy && copy->line)
	{
		if (copy->line[0] && !ft_strncmp(str, copy->line, i))
		{
			if (ft_strlen(copy->line) > 1)
				lst_modone(origin, copy->line,
					rev_strjoiny(copy->line, &str[i + 2]));
			else
				lst_modone(origin, copy->line,
					rev_strjoiny(copy->line, &str[i + 1]));
			break ;
		}
		copy = copy->next;
	}
	if (!copy)
		lst_addback(origin, tmp);
}

int	export_remp(int i, char *str, t_env *mini_env, t_env *origin)
{
	t_env	*temp;
	t_env	*copy;

	copy = origin;
	temp = mini_env;
	while (temp && temp->line)
	{
		if (ft_strlen(temp->line) > i && !ft_strncmp(str, temp->line, i + 1))
		{
			lst_modone(mini_env, temp->line, ft_strdup(str));
			break ;
		}
		temp = temp->next;
	}
	if (!temp || !temp->line)
		lst_addback(mini_env, str);
	while (copy && copy->line)
	{
		if (copy->line[0] && !ft_strncmp(str, copy->line, i))
			return (lst_modone(origin, copy->line, ft_strdup(str)), 0);
		copy = copy->next;
	}
	if (!copy || !copy->line)
		lst_addback(origin, str);
	return (0);
}

int	export_remp2(int i, char *str, t_env *origin)
{
	t_env	*copy;

	copy = origin;
	while (copy && copy->line)
	{	
		if (ft_strlen(copy->line) >= i && copy->line[i] == '='
			&& !ft_strncmp(str, copy->line, i))
			return (0);
		copy = copy->next;
	}
	copy = origin;
	while (copy && copy->line)
	{
		if (ft_strlen(copy->line) >= i && !ft_strncmp(str, copy->line, i))
		{
			lst_modone(origin, copy->line, ft_strdup(str));
			break ;
		}
		copy = copy->next;
	}
	if (!copy)
		lst_addback(origin, str);
	return (1);
}

int	ft_atoi(char *str, int *e)
{
	long	p;
	int		m;
	int		i;

	i = 0;
	p = 0;
	m = 1;
	if (!str)
		return (0);
	while ((str[i] <= 13 && str[i] >= 9) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			m = -m;
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		p = p * 10;
		p = p + (str[i] - 48);
		i++;
	}
	*e = i;
	return (p * m);
}
