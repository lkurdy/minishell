/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rben-tkh <rben-tkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 16:47:42 by rben-tkh          #+#    #+#             */
/*   Updated: 2022/09/21 20:24:17 by rben-tkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define SET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"

/*--------------------------------------LIBS---------------------------------*/

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>

/*-------------------------------------GLOBALE--------------------------------*/

extern int	g_ecode;

/*-------------------------------------S_INPUT--------------------------------*/

typedef struct s_input
{
	char	*s1;
	char	*s2;
}	t_input;

/*-------------------------------------E_CODES--------------------------------*/

typedef enum e_codes
{
	ECH = 1,
	CD = 2,
	PWD = 3,
	EXPORT = 4,
	UNSET = 5,
	ENV = 6,
	EXIT = 7,
	PIPE = 8,
	IN = 9,
	OUT = 10,
	DBIN = 11,
	DBOUT = 12,
}	t_codes;

/*-------------------------------------S_ENV---------------------------------*/

typedef struct s_env
{
	char			*line;
	struct s_env	*next;
}	t_env;

/*-------------------------------------S_DATA--------------------------------*/

typedef struct s_data
{
	int				code;
	char			*cmd;
	char			**args;
	int				in;
	int				out;
	int				a;
	int				b;
	int				exit;
	char			*path;
	struct s_data	*next;
	struct s_data	*prev;
}	t_data;

/*----------------------------------BUILTINS_UTILS---------------------------*/

void	shlvl(t_env *mini);
void	relou_remp(t_env *origin, char *str, int i, char *tmp);
int		export_remp(int i, char *str, t_env *mini_env, t_env *origin);
int		export_remp2(int i, char *str, t_env *origin);
int		ft_atoi(char *str, int *e);

/*-------------------------------------BUILTINS------------------------------*/

int		cmd_parse(t_data *data, t_env *mini, t_env *origin);

/*----------------------------------------CD---------------------------------*/

int		pre_cd(t_data *data, t_env *mini, t_env *origin);

/*---------------------------------------ECHO--------------------------------*/

char	*ft_strjoiny(char *s1, char *s2);
char	*rev_strjoiny(char *s1, char *s2);
char	*ft_strdupy(char *s);
void	ft_echo(char *str);
void	echo_remp(char *str, int opt, int i);

/*---------------------------------------ENV---------------------------------*/

void	ft_exit(t_data *data, t_env *mini, t_env *origin, char **args);
t_env	*ft_env(char **env);
void	ft_printenv(t_env *mini_env);
void	ft_pwd(void);

/*--------------------------------------EXPORT-------------------------------*/

void	lst_sort(t_env *origin);
void	lst_export(t_env *origin);
int		lst_exportarg(t_env *mini_env, t_env *origin, char *str);

/*-------------------------------------INIT_ENV------------------------------*/

void	lst_modone(t_env *mini_env, char *str, char *new);
void	lst_freeall(t_env *mini_env);
void	lst_delone(t_env *mini_env, char *env);
int		lst_addback(t_env *mini_env, char *env);
char	**ft_envp(t_env *mini);

/*---------------------------------------CMD---------------------------------*/

void	exec_loop(t_env *mini, t_env *origin, t_data *data);

/*------------------------------------EXEC_UTILS-----------------------------*/

char	**ft_parse(char **envp);
void	exe_remp(char **o, char **envp);
void	clean_dup(int out, int in);
void	pipex_remp(int *fd, t_data *tmp);

/*-------------------------------------FT_SPLIT------------------------------*/

int		ft_strlen(char *str);
char	**ft_split(const char *s, char c);

/*-------------------------------------FT_SPLITY-----------------------------*/

char	**ft_splity(const char *s, char c);
char	**fuse(char *cmd, char **args);

/*-------------------------------------GET_INPUT-----------------------------*/

int		get_promptlen(char *str, t_env *env, int val);
void	get_dollar(t_input *input, t_env *env, int *i, int *j);
int		get_input(t_input *input, t_env *env, int i, int j);

/*-------------------------------------IS_UTILS------------------------------*/

int		is_inquotes(char *str, int pos);
int		is_token(char *str, int i);
char	*is_dollar(char *str, int i);
char	*is_content(char *line);
int		is_ecode(void);

/*------------------------------------SPLIT_INPUT----------------------------*/

char	**split_input(char *str);
void	ft_free(char **dest, t_data **data);

/*---------------------------------------DATA--------------------------------*/

t_data	*data_create(char *prompt);
void	data_delelem(t_data **data, int i, int j);
void	data_init(t_data *data, t_env *env, t_env *origin);

/*-------------------------------------INIT_DATA-----------------------------*/

void	init_redi(t_data *redi, t_env *env, t_env *origin);
void	init_args(t_data *data);
void	init_cmdset(t_data **data, t_env *env, t_env *origin);

/*------------------------------------PARSE_UTILS----------------------------*/

void	utils_irdouky(t_data *redi, t_env *env, t_env *origin, char *s1);
char	*utils_irdouk(char *str, t_env *env, int i, int j);
void	utils_deltmp(t_data *data, t_data **tmp, t_data **head);
void	utils_next(t_data **data, t_data **head);

/*--------------------------------------PARSING------------------------------*/

int		check_quotes(char *str);
t_codes	check_code(char *str);
char	*check_dollar(t_env *env, char *str, int i);
t_data	*check_errors(t_input *input, t_env *env);

/*--------------------------------------LIBFT--------------------------------*/

char	*ft_strchr(const char *s, int c);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(char *s1, char *s2, int n);
char	*ft_strdup(char *s);
char	*ft_strjoin(char *s1, char *s2);

/*--------------------------------------SIGNAL-------------------------------*/

void	signal_int(int unused);
void	signal_quit(int unused);
void	signal_irdouk(char *str);

/*---------------------------------------T^T---------------------------------*/

#endif
