#include "../pipex.h"

void	error_exit(char *s, int error_nb)
{
	ft_putstr_fd(s, STDERR_FILENO);
	exit(error_nb);
}

void	cmd_error_exit(char **cmd, char *s, int error_nb)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd[0], STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	free_arrays(cmd);
	exit(error_nb);
}

void	check_cmd_error(char **s_cmd)
{
	int	len;

	if (!s_cmd || s_cmd[0] == NULL)
	{
		free_arrays(s_cmd);
		cmd_error_exit(s_cmd, ": command not found\n", 127);
	}
	if (s_cmd[0][0] == '.' || s_cmd[0][0] == '/')
	{
		len = ft_strlen(s_cmd[0]);
		if (s_cmd[0][len - 1] == '/')
		{
			ft_putstr_fd("pipex: ", STDERR_FILENO);
			ft_putstr_fd(s_cmd[0], STDERR_FILENO);
			free_arrays(s_cmd);
			error_exit(" is a directory", 126);
		}
		if (access(s_cmd[0], F_OK) == -1)
			cmd_error_exit(s_cmd, ": No such file or directory\n", 127);
		if (access(s_cmd[0], X_OK) == -1)
			cmd_error_exit(s_cmd, ": Permission denied\n", 126);
	}
}
