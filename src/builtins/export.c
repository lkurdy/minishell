/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:43:16 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/30 11:30:31 by lkurdy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lst_sort(t_env *origin)
{
	char	*temp;
	t_env	*copy;

	copy = origin;
	while (copy->next)
	{
		if (ft_strncmp(copy->line, copy->next->line,
				ft_strlen(copy->line) + ft_strlen(copy->next->line)) > 0)
		{
			temp = copy->line;
			copy->line = copy->next->line;
			copy->next->line = temp;
			copy = origin;
		}
		else
			copy = copy->next;
	}
	copy = origin;
	while (copy)
	{
		if (copy->line[ft_strlen(copy->line) - 1] != '"')
			copy->line = ft_strjoiny("", copy->line);
		copy = copy->next;
	}
}

void	lst_export(t_env *origin)
{
	t_env	*temp;

	temp = origin;
	while (temp)
	{
		printf("declare -x ");
		printf("%s\n", temp->line);
		temp = temp->next;
	}
}

static int	exp_err(char *str)
{
	write(2, "minihlel: export: `", 19);
	write(2, str, ft_strlen(str));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

static int	relou(t_env *mini, t_env *origin, char *str, int i)
{
	t_env	*temp;
	char	*tmp;

	tmp = ft_strdupy(str);
	temp = mini;
	while (temp && temp->line)
	{
		if (ft_strlen(temp->line) >= i && !ft_strncmp(str, temp->line, i))
		{
			lst_modone(mini, temp->line, rev_strjoiny(temp->line, &str[i + 2]));
			break ;
		}
		temp = temp->next;
	}
	if (!temp)
		lst_addback(mini, tmp);
	relou_remp(origin, str, i, tmp);
	return (free(tmp), 0);
}

int	lst_exportarg(t_env *mini_env, t_env *origin, char *str)
{
	int		i;

	i = 0;
	while (str[i] && str[i] == '=')
			i++;
	if (!str[i] || (str[0] > '0' && str[0] < '9'))
		return (exp_err(str));
	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '-' && str[i] != '+')
		i++;
	if (str[i])
	{
		if (str[i] == '-')
			return (exp_err(str));
		if (i > 0 && str[i] == '+' && str[i + 1] == '=')
			return (relou(mini_env, origin, str, i));
		else if (str[i] == '+')
			return (exp_err(str));
		export_remp(i, str, mini_env, origin);
	}
	else
		export_remp2(i, str, origin);
	return (0);
}
