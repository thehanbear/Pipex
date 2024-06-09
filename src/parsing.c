#include "../pipex.h"

char	**find_env(char **envp)
{
	char	*path_var;

	path_var = NULL;
	if (*envp == NULL)
		return (ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':'));
	while (*envp)
	{
		if (ft_strncmp("PATH=", *envp, 5) == 0)
		{
			path_var = *envp + 5;
			break ;
		}
		envp++;
	}
	if (!path_var)
		return (NULL);
	return (ft_split(path_var, ':'));
}

char	*get_path(char **cmd, t_pipex *pipex)
{
	char	*temp;
	char	*path;
	int		i;

	i = 0;
	if (!pipex->path)
		cmd_error_exit(cmd, ": No such file or directory\n", 127);
	while (pipex->path[i])
	{
		temp = ft_strjoin(pipex->path[i], "/");
		path = ft_strjoin(temp, cmd[0]);
		if (temp)
			free(temp);
		if (access(path, F_OK) == 0)
			break ;
		if (path)
			free(path);
		path = NULL;
		i++;
	}
	return (path);
}

void	check_null_cmd(char *argv)
{
	int	len;
	int	i;
	int	found;

	found = 0;
	i = -1;
	len = ft_strlen(argv);
	while (++i < len)
	{
		if (argv[i] != ' ')
		{
			found = 1;
			break ;
		}
	}
	if (found == 0)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(argv, STDERR_FILENO);
		error_exit(": command not found\n", 127);
	}
}

char	**prepare_cmd(char *argv)
{
	int		i;
	char	**cmd;

	if (!argv)
		error_exit("Argv error\n", 127);
	argv = remove_quotes(argv);
	check_null_cmd(argv);
	cmd = split_cmd(argv);
	i = -1;
	while (cmd[++i])
		cmd[i] = remove_quotes(cmd[i]);
	check_cmd_script(cmd);
	check_cmd_error(cmd);
	return (cmd);
}

void	execute_cmd(char *argv, t_pipex *pipex, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = prepare_cmd(argv);
	if (ft_strrchr(cmd[0], '/') != NULL)
		path = cmd[0];
	else
	{
		path = get_path(cmd, pipex);
		if (!path)
			cmd_error_exit(cmd, ": command not found\n", 127);
	}
	check_path(path, cmd);
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		cmd_error_exit(cmd, ": command not found\n", 127);
	}
}
