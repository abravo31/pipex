/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abravo <abravo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 22:18:40 by abravo            #+#    #+#             */
/*   Updated: 2022/12/28 20:09:35 by abravo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**get_paths(char **env)
{
	char	**paths;
	int		i;

	i = 0;
	while (ft_strnstr(env[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(env[i] + 5, ':');
	return (paths);
}

static char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*part_path;
	int		i;

	i = 0;
	if (ft_strnstr(cmd, "/", ft_strlen(cmd)))
		return (cmd);
	paths = get_paths(env);
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_args(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_args(paths);
	return (0);
}

void	ft_exec(char *av, char **env)
{
	char	**cmd;
	char	*path;
	int		i;

	i = -1;
	cmd = ft_split(av, ' ');
	path = find_path(cmd[0], env);
	if (!path)
	{
		cmd_not_found(cmd[0]);
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		exit(127);
	}
	execve(path, cmd, env);
	perror(cmd[0]);
	free_args(cmd);
}
