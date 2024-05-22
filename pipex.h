#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_pipex
{
	pid_t	ch1;
	pid_t	ch2;
	int		fd[2];
	int		infile;
	int		outfile;
	char	**cmd1;
	char	**cmd2;
}	t_pipex;


# endif
