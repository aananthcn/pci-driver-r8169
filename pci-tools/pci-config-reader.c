#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros
#define MAXLINES	(256)
#define PDEV_ADDR_LEN	(32)


// Function to execute a system command and collect output
void execute_command(const char *cmd, char *output[], int maxLines)
{
	FILE *pipe = popen(cmd, "r");
	if (!pipe)
	{
		perror("popen() failed");
		exit(EXIT_FAILURE);
	}

	char buffer[256];
	int lineCount = 0;

	while (fgets(buffer, sizeof(buffer), pipe) != NULL && lineCount < maxLines)
	{
		// Remove newline character from the end of each line
		buffer[strcspn(buffer, "\n")] = '\0';

		// Allocate memory for the line and copy the content
		output[lineCount] = strdup(buffer);
		lineCount++;
	}

	pclose(pipe);
}


void select_device(char *output[], char dev_addr[]) {
	int device_num;

	if ((output == NULL) || (dev_addr == NULL)) {
		printf("ERROR: %s(): invalid arguments!\n", __func__);
		exit(1);
	}

	for (int i = 0; i < MAXLINES && output[i] != NULL; i++)
	{
		printf("[%2d] %s\n", i, output[i]);
	}
	printf("\n\nType the device number and press ENTER: ");
	scanf("%d", &device_num);
	strcpy(dev_addr, strtok(output[device_num], " "));


	// free the memory created by strdup() here, to avoid memory leak
	for (int i = 0; i < MAXLINES && output[i] != NULL; i++)
	{
		free(output[i]); // Free allocated memory
	}
}


int main()
{
	char *output[MAXLINES] = { NULL };
	char command[MAXLINES];
	char pdev_addr[PDEV_ADDR_LEN];

	strcpy(command, "lspci");
	execute_command(command, output, MAXLINES);
	select_device(output, pdev_addr);

	printf("\nDevice selected is: %s\n", pdev_addr);


	return 0;
}
