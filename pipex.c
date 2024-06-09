#include "../pipex.h"

void	pipex_init(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = 0;
	pipex->outfile = 0;
	pipex->pid1 = 0;
	pipex->pid2 = 0;
	pipex->path = find_env(envp);
	pipex->status = 0;
	pipex->cmd1 = argv[2];
	pipex->cmd2 = argv[3];
}

void	first_child(char **argv, int fd[2], char **envp, t_pipex *pipex)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
	{
		if (access(argv[1], F_OK) == 0)
			error_exit("pipex: input: Permission denied", 1);
		else
			error_exit("pipex: input: No such file or directory\n", 1);
	}
	close(fd[0]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		error_exit("dup2 error\n", 1);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_exit("dup2 error\n", 1);
	close(pipex->infile);
	close(fd[1]);
	execute_cmd(pipex->cmd1, pipex, envp);
}

void	second_child(char **argv, int fd[2], char **envp, t_pipex *pipex)
{
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
	{
		if (access(argv[4], W_OK) == -1)
			error_exit("pipex: output: Permission denied", 1);
		else
			error_exit("pipex: output: No such file or directory\n", 1);
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == 1)
		error_exit("dup2 error\n", 1);
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		error_exit("dup2 error\n", 1);
	close(fd[0]);
	close(pipex->outfile);
	execute_cmd(pipex->cmd2, pipex, envp);
}

int	pipex_process(char **argv, t_pipex *pipex, char **envp)
{
	int	fd[2];

	if (pipe(fd) == -1)
		error_exit("pipe error\n", 1);
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		error_exit("fork error\n", 1);
	if (pipex->pid1 == 0)
		first_child(argv, fd, envp, pipex);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		error_exit("fork error\n", 1);
	if (pipex->pid2 == 0)
		second_child(argv, fd, envp, pipex);
	close(fd[1]);
	close(fd[0]);
	if (waitpid(pipex->pid1, NULL, 0) == -1)
		error_exit("waitpid error\n", 1);
	if (waitpid(pipex->pid2, &pipex->status, 0) == -1)
		error_exit("waitpid error\n", 1);
	return (pipex->status);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	if (argc != 5)
		error_exit("Wrong amount of arguments\n", 1);
	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		error_exit("allocation error\n", 1);
	pipex_init(pipex, argv, envp);
	pipex->status = pipex_process(argv, pipex, envp);
	if (WIFEXITED(pipex->status))
		exit(WEXITSTATUS(pipex->status));
	free_close_all(pipex);
	return (0);
}
