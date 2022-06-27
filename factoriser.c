#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * ck_and_rd_file - checks input and reads file
 * @argc: int; argument counter
 * @argv: double pointer to char; argument vector holding name of file
 *
 * Return: pointer to char; buffer with content of given file
 * if failure return NULL
 * TheOwl
 */
char *ck_and_rd_file(int argc, char **argv)
{
	char *buffer;
	int buf_size, fd;
	ssize_t l_read;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "USAGE: factors file\n");
		return (NULL);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't open file %s\n", argv[1]);
		return (NULL);
	}
	/* get file size and malloc buffer for it */
	buf_size = lseek(fd, 0, SEEK_END);
	if (buf_size == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't get size of file %s\n", argv[1]);
		return (NULL);
	}
	buffer = malloc((buf_size + 1) * sizeof(char));
	if (buffer == NULL)
	{
		dprintf(STDERR_FILENO, "Error: malloc failed\n");
		return (NULL);
	}
	lseek(fd, 0, SEEK_SET); /* reset offset and read monty file */
	l_read = read(fd, buffer, buf_size);
	if (l_read == -1)
	{
		free(buffer);
		dprintf(STDERR_FILENO, "Error : can't read file %s\n", argv[1]);
		return (NULL);
	}
	buffer[buf_size] = '\0';
	close(fd);
	return (buffer);
}

/**
 * line_extract - extracts lines from a string
 * @str: pointer to char; string to process
 *
 * Return: double pointer to char; list of lines in string
 * TheOwl
 */
char **line_extract(char *str)
{
	int i = 0, j = 0, k = 0, l = 0;
	char **lines;

	/* get number of lines and malloc space for them */
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
			j++;
		i++;
	}
	lines = malloc((j + 2) * sizeof(char *));
	if (lines == NULL)
		return (NULL);
	/* malloc space and copy data from str to lines */
	lines[j + 1] = NULL;
	i = 0;
	while (str[i] != '\0')
	{
		for (j = 0; str[j] != '\n';)
			j++;
		lines[k] = malloc((j + 1) * sizeof(char));
		if (lines[k] == NULL)
			return NULL;
		lines[k][j] = '\0';
		for (l = 0; str[i] != '\n'; l++)
		{
			lines[k][l] = str[i];
			i++;
		}
		i++, k++;
	}
	return (lines);
}

/**
 * factorizer - finds the factors
 * @num: long long unsigned int, number
 *
 * Return: nothing
 * TheOwl
 */
void factorizer(long long unsigned int num)
{
	long long unsigned int i = 2;

	while (i < (num / 2 + 1))
	{
		if (num % i == 0)
		{
			printf("%lld=%lld*%lld\n", num, i, num / i);
			return;
		}
		i++;
	}
	printf("%lld=1*%lld\n", num, num);
	return;
}

/**
 * main - entry point
 * print factorisation of semi prime numbers of a given file
 * argc: int; argument counter
 * argv: double pointer to char; argument vector holding name of given file
 *
 * Return: 1 if success, 0 if failure
 * TheOwl
 */
int main(int argc, char **argv)
{
	char *buffer;
	char **line_ls;
	int i = 0;

	buffer = ck_and_rd_file(argc, argv);
	if (buffer == NULL)
		return (0);
	line_ls = line_extract(buffer);
	if (line_ls == NULL)
		return (0);
	while (line_ls[i] != NULL)
	{
		factorizer(atoll(line_ls[i]));
		i++;
	}
	for (i = 0; line_ls[i] != NULL; i++)
		free(line_ls[i]);
	free(line_ls);
	return (1);
}
