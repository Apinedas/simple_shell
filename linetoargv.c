#include "shell.h"

/**
 * _getenv - Extracts an env variable from environ
 * @name: Variable name
 * Return: Pointer to variable name, or NULL if variable hasn't be found
 */

char *_getenv(char *name)
{
	char *extractenv;
	int difference, i, envlen;

	for (i = 0; environ[i]; i++)
	{
		envlen = _strlen(environ[i]);
		extractenv = malloc(sizeof(*extractenv) * (envlen + 1));
		if (extractenv == NULL)
			return (NULL);
		extractenv = _strcpy(extractenv, environ[i]);
		extractenv = strtok(extractenv, "=");
		difference = _strcmp(name, extractenv);
		if (difference == 0)
		{
			free(extractenv);
			return (environ[i]);
		}
		else
			free(extractenv);
	}
	return (NULL);
}

/**
 * manage_path - Search for a file in the PATH
 * @file: String to file to search
 * Return: Path to file if found, or NULL if not found in any path
 */

char *manage_path(char *file)
{
	char *path, *direction, *auxpath;
	struct stat st;
	int pathlen, filelen, filestatus;

	pathlen = _strlen(_getenv("PATH"));
	path = _calloc((pathlen + 1), sizeof(*path));
	if (path == NULL)
		return (NULL);
	auxpath = path;
	_strcpy(path, _getenv("PATH"));
	path = strtok(path, "=");
	path = strtok(NULL, "=");
	path = strtok(path, ":");
	filelen = _strlen(file);
	while (path)
	{
		direction = _calloc((pathlen + filelen + 2), sizeof(*direction));
		if (direction == NULL)
			return (NULL);
		_strcpy(direction, path);
		if (direction[pathlen - 1] != '/')
		{
			direction[pathlen] = '/';
			direction[pathlen + 1] = '\0';
		}
		direction = _strcat(direction, file);
		filestatus = stat(direction, &st);
		if (filestatus == 0)
		{
			free(auxpath);
			return (direction);
		}
		free(direction);
		path = strtok(NULL, ":");
		pathlen = _strlen(path);
	}
	free(auxpath);
	return (NULL);
}

/**
 * linetoargv - transforms a read line in an argv array
 * @line: Line to transform into argv
 * @argv: Array to put line into
 * @linelen: length of line to transform
 * Return: Argv filled with Modified line
 */

char **linetoargv(char *line, char **argv, ssize_t linelen)
{
	char *auxline;
	struct stat st;
	int i, filestatus;

	line[linelen - 1] = '\0';
	auxline = strtok(line, " ");
	for (i = 0; auxline; i++)
	{
		argv[i] = auxline;
		auxline = strtok(NULL, " ");
	}
	argv[i] = NULL;
	filestatus = stat(argv[0], &st);
	if (filestatus == 0)
		return (argv);
	auxline = manage_path(argv[0]);
	if (auxline != NULL)
		argv[0] = auxline;
	else
		argv[0] = NULL;
	return (argv);
}
