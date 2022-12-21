/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abravo <abravo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 23:13:19 by abravo            #+#    #+#             */
/*   Updated: 2022/12/21 20:45:27 by abravo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_pipe()
{
	int	fd[2];
	int	id;
	int	x;
	int	y;
	//fd[0] - read
	//fd[1] - write
	//void (ac);
	//void (av);

	if (pipe(fd) == -1)
	{
		printf("An error ocurre with opening the pipe\n");
		return ;
	}
	id = fork();
	if (id == 0)
	{
		close(fd[0]);
		printf("Input number: ");
		scanf("%d", &x);
		write(fd[1], &x, sizeof(int));
		//check error if write ruturn -1
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		read(fd[0], &y, sizeof(int));
		//check error if read return -1
		close(fd[0]);
		printf("Got from child process %d\n", y);
	}
}

void	while_fork()
{
	int	id1;
	int	id2;

	id1 = fork();
	id2 = fork();
	if (id1 == 0)
	{
		if (id2 == 0)
			printf("we are process y\n");
		else
			printf("we are process x\n");
	}
	else
	{
		if (id2 == 0)
			printf("we are process z\n");
		else
			printf("we are the parent process\n");
	}
	while (wait(NULL) != -1 || errno != ECHILD)
		printf("Waited for a child to finish\n");
}

void	get_pid(int id)
{
	int	n;
	int	i;

	if (id == 0)
		sleep(2);
	printf("Current ID: %d, parent ID: %d\n", getpid(), getppid());
	if (id == 0)
		n = 1;
	else
		n = 6;
	if (id != 0)
		wait(0);
	i = n;
	while (i < n + 5)
	{
		printf("%d ", i);
		i++;
	}
	if (id != 0)
		printf("\n");
}

void	hello_from_child(int id)
{
	int	res;

	if (id == 0)
		printf("Hello from child process\n");
	else
		printf("Hello from the main process\n");
	res = wait(NULL);
	if (res == -1)
		printf("No children to wait for\n");
	else
		printf("%d finished execution\n", res);
}

int	print_sum_parent(int size, int *tab, int *fd)
{
	int	start;
	int	end;
	int	sum;

	start = 0;
	end = size / 2;
	sum = 0;
	while (start < end)
	{
		sum += tab[start];
		start++;
	}
	printf("Calculeted partial parent sum : %d\n", sum);
	close(fd[0]);
	write(fd[1], &sum, sizeof(sum));
	close(fd[1]);
	return (sum);
}

int	print_sum_child(int size, int *tab, int *fd)
{
	int	start;
	int	end;
	int	sum;

	start = size / 2;
	end = size;
	sum = 0;
	while (start < end)
	{
		sum += tab[start];
		start++;
	}
	printf("Calculeted partial child sum : %d\n", sum);
	close(fd[1]);
	write(fd[0], &sum, sizeof(sum));
	//check if write = -1
	close(fd[0]);
	return (sum);
}

int	sum_array(int *arr)
{
	int	size;
	int	fd[2];
	int	id;
	int	sum;

	size = sizeof(arr) / sizeof(int);
	if (pipe(fd) == -1)
	{
		printf("An error ocurre with opening the pipe\n");
		return (0);
	}
	id = fork();
	if (id == -1)
		printf("Error\n");
	if (id == 0)
		print_sum_parent(size, arr, fd);
	else
	{
		print_sum_child(size, arr, fd);
		wait(NULL);
	}
	sum = print_sum_parent(size, arr, fd) + print_sum_child(size, arr, fd);
	return (sum);
}

int	main(int ac, char **av, char **env)
{
	int		fd[2];
	pid_t	pid1;

	if (!*env)
	{
		ft_putstr_fd("Error\n", 2);
		return (0);
	}
	if (ac == 5)
	{
		if (pipe(fd) == -1)
			ft_error(NULL);
		pid1 = fork();
		if (pid1 == -1)
			ft_error(NULL);
		if (pid1 == 0)
			child_process();
		else
			parent_process();
	}
	else
		write(2, "Invalid number of arguments.\n", 29);
	return (0);
}
