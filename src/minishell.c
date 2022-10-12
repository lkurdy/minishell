/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 17:06:18 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/21 10:56:57 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ecode = 0;

void	signal_int(int unused)
{
	(void)unused;
	rl_on_new_line();
	write(2, "\n", 1);
	rl_replace_line("", 0);
	if (g_ecode > 0)
		rl_redisplay();
	g_ecode = 130;
}

void	signal_quit(int unused)
{
	(void)unused;
	write(2, "Quit (code dumped)\n", 19);
	g_ecode = 131;
}

void	signal_irdouk(char *str)
{
	write(2, "minihlel: warning: ", 19);
	write(2, "ir-doukoumen delimited by end-of-file", 37);
	if (str)
	{
		write(2, " (wanted `", 10);
		write(2, str, ft_strlen(str));
		write(2, "')\n", 3);
	}
	g_ecode = 0;
}

void	minishell(t_env *mini, t_env *origin, char *str)
{
	t_input	input;
	t_data	*data;
	int		i;

	if (!str)
		return ;
	i = 0;
	while (str && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str && str[i])
	{
		input.s1 = ft_strdup(str);
		input.s2 = 0;
		free(str);
		data = check_errors(&input, mini);
		init_cmdset(&data, mini, origin);
		if (data)
			exec_loop(mini, origin, data);
		ft_free(0, &data);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*mini;
	t_env	*origin;
	char	*str;

	origin = ft_env(env);
	mini = ft_env(env);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_int);
	while (ac > 0 && av[0])
	{
		str = readline("minihlel> ");
		if (!str)
		{
			printf("exit\n");
			free(str);
			ft_exit(0, mini, origin, 0);
		}
		else if (str && str[0])
		{
			add_history(str);
			minishell(mini, origin, str);
		}
	}
	return (lst_freeall(origin), lst_freeall(mini), 0);
}
