#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

bool exitThread = false;
void chkErr(int r, const char *message)
{
	if (r == -1)
	{
		perror(message);
		exit(1);
	}
}

void *input_runner(void *s)
{
	long sock;
	sock = (long)s;
	while (!exitThread)
	{
		int x;
		char command[100] = {};

		//take input
		x = read(STDIN_FILENO, command, 100);
		chkErr(x, "read command");
		//write to server
		x = write(sock, command, strlen(command));
		chkErr(x, "write command to server");
	}
	pthread_exit(NULL);
}

void *output_runner(void *s)
{
	long sock;
	sock = (long)s;
	while (true)
	{
		char output[1000] = {};
		int x = read(sock, output, 1000);
		chkErr(x, "read output from server");
		if (x == 0)
		{
			write(STDOUT_FILENO, "Server terminated\n", 19);
			// exitThread = true;
			// pthread_exit(NULL);
			exit(0);
		}
		if (strcmp(output, "exit") == 0)
		{
			// exitThread = true;
			// pthread_exit(NULL);
			exit(0);
		}
		else
		{
			//output
			x = write(STDOUT_FILENO, output, x);

			chkErr(x, "write output");
		}
	}
}
int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		int x = write(STDOUT_FILENO, "Insufficient arguments. Enter IP and port.\n", 43);
		exit(1);
	}
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buf[1024];
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("opening stream socket");
		exit(1);
	}
	/* Connect socket using name specified by command line. */
	server.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == 0)
	{
		fprintf(stderr, "%s: unknown host\n", argv[1]);
		exit(2);
	}
	bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connecting stream socket");
		exit(1);
	}
	write(STDOUT_FILENO, "Enter commands on the terminal.\nPossible commands:\n****add <list<int>>\n****sub <list<int>>\n****mul <list<int>>\n****div <list<int>>\n****run <path>\n****list\n****list all\n****kill <process name>\n****kill <process id>\n****exit\n", 224);

	pthread_t input_thread;
	pthread_t output_thread;

	int rc = pthread_create(&input_thread, NULL, input_runner, (void *)sock);
	if (rc)
	{
		perror("create input thread");
		exit(1);
	}
	rc = pthread_create(&output_thread, NULL, output_runner, (void *)sock);
	if (rc)
	{
		perror("create output thread");
		exit(1);
	}
	pthread_join(input_thread, NULL);
	pthread_join(output_thread, NULL);
	close(sock);

	return 0;
}