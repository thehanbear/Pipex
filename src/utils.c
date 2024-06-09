#include "../pipex.h"

void	check_path(char *path, char **cmd)
{
	if (access(path, F_OK) == -1)
		cmd_error_exit(cmd, ": command not found\n", 127);
	if (access(path, X_OK) == -1)
		cmd_error_exit(cmd, ": Permission denied\n", 126);
}

void	check_cmd_script(char **s_cmd)
{
	int	len;

	len = ft_strlen(s_cmd[0]);
	if (s_cmd[0][len - 3] == '.' &&
		s_cmd[0][len - 2] == 's' && s_cmd[0][len - 1] == 'h')
	{
		if (ft_strrchr(s_cmd[0], '/') == NULL)
		{
			cmd_error_exit(s_cmd, ": command not found\n", 127);
		}
		if (ft_strrchr(s_cmd[0], '/') != NULL)
			check_path(s_cmd[0], s_cmd);
	}
}

char	*remove_quotes(char *s)
{
	int		len;
	char	*new_s;

	len = ft_strlen(s);
	if ((s[0] == '"' && s[len - 1] == '"')
		|| (s[0] == '\'' && s[len - 1] == '\''))
	{
		new_s = malloc((len - 1) * sizeof(char));
		if (!new_s)
		{
			free(new_s);
			error_exit("Memory allocation failed\n", 1);
		}
		ft_strncpy(new_s, s + 1, len - 2);
		new_s[len - 2] = '\0';
		free(s);
		return (new_s);
	}
	return (s);
}

void	free_arrays(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
}

void	free_close_all(t_pipex *pipex)
{
	if (pipex->infile > 0)
		close(pipex->infile);
	if (pipex->outfile > 0)
		close(pipex->outfile);
	free(pipex);
}
