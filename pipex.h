#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		status;
	pid_t	pid1;
	pid_t	pid2;
	char	**path;
	char	*cmd1;
	char	*cmd2;
}	t_pipex;

int		main(int argc, char **argv, char **envp);
void	error_exit(char *s, int error_nb);
int		pipex_process(char **argv, t_pipex *pipex, char **envp);
void	first_child(char **argv, int fd[2], char **envp, t_pipex *pipex);
void	second_child(char **argv, int fd[2], char **envp, t_pipex *pipex);
void	execute_cmd(char *argv, t_pipex *pipex, char **envp);
void	check_path(char *path, char **cmd);
void	free_arrays(char **str);
void	pipex_init(t_pipex *pipex, char **argv, char **envp);
void	free_close_all(t_pipex *pipex);
char	*remove_quotes(char *s);
char	*get_path(char **cmd, t_pipex *pipex);
char	**find_env(char **envp);
char	**split_cmd(char const *str);
void	check_cmd_error(char **s_cmd);
void	check_cmd_script(char **s_cmd);
void	cmd_error_exit(char **cmd, char *s, int error_nb);
char	**prepare_cmd(char *argv);

#endif

