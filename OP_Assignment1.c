/*
 * Operating Systems - Assignment #1
 * Author: Daniel Tal
 * ID: 300488228
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 512

int main()
{
	int stat, pid, wordCount, i;
	char *cmd, **args, *currArgs, *pPath, *cPath, *fullPath;

	while (1)
	{
		cmd = (char*)malloc(sizeof(char) * N);
		printf("DanielTal$ > ");
		gets(cmd);
		if (!(strcmp(cmd, "leave")))
		{
			free(cmd);
			printf("GoodBye\n");
			exit(0);
		}

		wordCount = 1;

		for (i = 0; cmd[i] != '\0'; i++)
		{
			if (cmd[i] == ' ')
			{
				wordCount++;
				while (cmd[i + 1] == ' ')
				{
					if (cmd[i + 2] == '\0')
						wordCount--;
					else
						i++;
				}
			}
		}

		currArgs = strtok(cmd, " ");
		args = (char**)malloc(sizeof(char*) * wordCount + 1);

		for (i = 0; i < wordCount; i++)
		{
			args[i] = (char*)malloc(strlen(currArgs));
			strcpy(args[i], currArgs);
			currArgs = strtok(NULL, " ");
		}

		args[wordCount] = NULL;

		if ((pid = fork()) == 0)
		{
			execv(cmd, args);

			pPath = (char*)getenv("PATH");
			cPath = strtok(pPath, ":");

			while (cPath)
			{
				fullPath = (char*)malloc(strlen(cmd) + strlen(cPath) + 2);
				strcpy(fullPath, cPath);
				strcat(fullPath, "/");
				strcat(fullPath, cmd);

				execv(fullPath, args);

				free(fullPath);
				cPath = strtok(NULL, ":");
			}
			printf("command not found\n");
			exit(1);
		}
		else
		{
			wait(&stat);
			for (i = 0; i < wordCount; i++)
				free(args[i]);
			free(args);
			free(cmd);
		}
	}
	exit(0);
}
