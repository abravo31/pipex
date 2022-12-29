/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abravo <abravo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 23:13:19 by abravo            #+#    #+#             */
/*   Updated: 2022/12/28 20:06:17 by abravo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_command_process(char **av, int *fd, char **env)
{
	int		fd_in;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_error(NULL);
	if (pid == 0)
	{
		fd_in = (open(av[1], O_RDONLY, 0644));
		if (fd_in == -1)
		{
			ft_error(av[1]);
			return ;
		}
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		dup2(fd_in, STDIN_FILENO);
		close(fd[1]);
		ft_exec(av[2], env);
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
}

static void	second_command_process(char **av, int *fd, char **env)
{
	int		fd_out;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_error(NULL);
	if (pid == 0)
	{
		fd_out = (open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644));
		if (fd_out == -1)
		{
			ft_error(av[4]);
			return ;
		}
		close(fd[1]);
		// dup2(fd[0], STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd[0]);
		ft_exec(av[3], env);
	}
}

int	main(int ac, char **av, char **env)
{
	int		fd[2];

	if (!*env)
	{
		ft_putstr_fd("Error\n", 2);
		return (0);
	}
	if (ac == 5)
	{
		if (pipe(fd) == -1)
			ft_error(NULL);
		first_command_process(av, fd, env);
		second_command_process(av, fd, env);
		while (waitpid(0, NULL, 0) > 0)
			continue ;
	}
	else
		write(2, "Invalid number of arguments.\n", 29);
	return (0);
}
