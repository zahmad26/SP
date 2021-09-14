#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <list>
#include <fcntl.h>
#include <signal.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <ctime>
#include <pthread.h>

#include <arpa/inet.h>
using namespace std;

#define TRUE 1

class Process
{
public:
	int pid;
	char pname[50];
	time_t start_time;
	time_t end_time = NULL;
	double elapsed_time;
};

class ClientHandler
{
public:
	int ch_pid;
	char ip[50];
	int conn_read_fd;
	int conn_write_fd;
	bool isActive = false;
};

struct RW
{
	int read;
	int write;
	int msgsock;
};

list<Process> allProcess = {};
list<ClientHandler> connList = {};
int child_pid = -1;

void chkErr(int r, const char *message)
{
	if (r == -1)
	{
		perror(message);
		exit(0);
	}
}

//list func
void listProcess(char list[])
{
	if (!allProcess.empty())
	{
		bool noActive = true;
		strcat(list, "pid    pname   start\n");
		for (Process i : allProcess)
		{
			if (i.end_time == NULL) //active
			{
				noActive = false;
				char b[100] = {};
				int check = sprintf(b, "%d  %s   %s\n", i.pid, i.pname, ctime(&i.start_time));
				chkErr(check, "sprintf ");
				strcat(list, b);
			}
		}
		if (noActive)
		{
			list[0] = '\0';
			strcat(list, "list is empty\n");
		}
	}
	else
	{
		strcat(list, "list is empty\n");
	}
}

//list all func
void listAllProcess(char list[])
{
	if (!allProcess.empty())
	{
		for (Process i : allProcess)
		{
			char b[100] = {};
			char c[40] = {};
			if (i.end_time != NULL) // closed
			{
				int check = sprintf(b, "pid  : %d\npname: %s\nstart: %s", i.pid, i.pname, ctime(&i.start_time));
				chkErr(check, "sprintf ");
				strcat(list, b);
				sprintf(c, "end  : %selapse: %fs\n\n", ctime(&i.end_time), i.elapsed_time);
				strcat(list, c);
			}
			else //active
			{
				int check = sprintf(b, "pid  : %d\npname: %s\nstart: %s", i.pid, i.pname, ctime(&i.start_time));
				chkErr(check, "sprintf ");
				strcat(list, b);
			}
		}
	}
	else
	{
		strcat(list, "list is empty\n");
	}
}

//thread function for conn I/O
void *conn_io_runner(void *s)
{
	write(STDOUT_FILENO, "Enter commands on the terminal.\nPossible commands:\n****connlist\n****connlist all\n****print <message>\n****print <client IP Address> <message>\n****list\n****list <client IP Address>\n", 180);

	while (1)
	{
		int x;
		char command[100] = {};

		//take conn input
		x = read(STDIN_FILENO, command, 100);
		chkErr(x, "read command");
		command[x - 1] = NULL;
		char *ptr = strtok(command, " ");

		if (ptr == NULL)
		{
			x = write(STDOUT_FILENO, "\n", 2);
			chkErr(x, "output to client");
		}
		else if (strcmp(ptr, "connlist") == 0)
		{
			ptr = strtok(NULL, " ");
			if (ptr == NULL) //list
			{
				char Clist[500] = {};
				if (!connList.empty())
				{
					bool noActive = true;
					strcat(Clist, "#  pid  IP\n");
					int j = 0;
					for (ClientHandler i : connList)
					{
						j++;
						if (i.isActive)
						{
							noActive = false;
							char b[100] = {};
							int check = sprintf(b, "%d  %d  %s\n", j, i.ch_pid, i.ip);
							chkErr(check, "sprintf ");
							strcat(Clist, b);
						}
					}
					if (!noActive)
					{
						int x = write(STDOUT_FILENO, Clist, strlen(Clist));
						chkErr(x, "write output Clist");
					}
					else
					{
						x = write(STDOUT_FILENO, "connection list is empty\n", 26);
						chkErr(x, "write ouput");
					}
				}
				else
				{
					x = write(STDOUT_FILENO, "connection list is empty\n", 26);
					chkErr(x, "write ouput");
				}
			}
			else if (strcmp(ptr, "all") == 0) //list all
			{
				char Clist[500] = {};
				if (connList.empty() == false)
				{
					strcat(Clist, "#  pid  IP\n");
					int j = 0;
					for (ClientHandler i : connList)
					{
						j++;
						char b[100] = {};
						int check = sprintf(b, "%d  %d  %s\n", j, i.ch_pid, i.ip);
						chkErr(check, "sprintf ");
						strcat(Clist, b);
					}
					int x = write(STDOUT_FILENO, Clist, strlen(Clist));
					chkErr(x, "write output Clist");
				}
			}
		}
		else if (strcmp(ptr, "list") == 0)
		{
			ptr = strtok(NULL, " ");
			if (ptr == NULL) //list of all clients active process
			{
				if (!connList.empty())
				{
					bool clientFound = false;
					for (ClientHandler i : connList)
					{
						clientFound = true;
						if (i.isActive)
						{
							clientFound = false;
							int x = write(i.conn_write_fd, "list", 4); //write command to conn handler
							chkErr(x, "write list comm to specific conn handler");
							char output[1000] = {};
							int y = read(i.conn_read_fd, output, 500); //read from conn handler
							chkErr(y, "read from conn handler");
							char clientID[35];
							sprintf(clientID, "*** client ID: %d ***\n", i.ch_pid);
							x = write(STDOUT_FILENO, clientID, strlen(clientID)); //write output
							chkErr(x, "write ouput");
							x = write(STDOUT_FILENO, output, y); //write output
							chkErr(x, "write ouput");
						}
					}
					if (clientFound)
					{
						x = write(STDOUT_FILENO, "No active process\n", 19);
						chkErr(x, "write ouput");
					}
				}
				else
				{
					x = write(STDOUT_FILENO, "connection list is empty\n", 26);
					chkErr(x, "write ouput");
				}
			}
			else // list of specific clients active process
			{
				if (!connList.empty())
				{
					bool clientFound = false;
					for (ClientHandler i : connList)
					{
						if (atoi(ptr) == i.ch_pid)
						{
							clientFound = true;
							if (i.isActive)
							{
								int x = write(i.conn_write_fd, "list", 4); //write command to conn handler
								chkErr(x, "write list comm to specific conn handler");
								char output[1000] = {};
								x = read(i.conn_read_fd, output, 1000); //read from conn handler
								chkErr(x, "read from conn handler");
								x = write(STDOUT_FILENO, output, x); //write output
								chkErr(x, "write ouput");
							}
						}
					}
					if (!clientFound)
					{
						x = write(STDOUT_FILENO, "No such client\n", 16);
						chkErr(x, "write ouput");
					}
				}
				else
				{
					x = write(STDOUT_FILENO, "connection list is empty\n", 26);
					chkErr(x, "write ouput");
				}
			}
		}
		else if (strcmp(ptr, "print") == 0)
		{
			if (!connList.empty())
			{
				bool isIP = false;
				ptr = strtok(NULL, " ");
				if (ptr != NULL)
				{
					for (ClientHandler i : connList)
					{
						if (atoi(ptr) == i.ch_pid)
						{
							if (i.isActive)
							{
								ptr = strtok(NULL, "");
								if (ptr != NULL)
								{
									int x = write(i.conn_write_fd, ptr, strlen(ptr)); //write message to conn handler
									chkErr(x, "write message to conn handler");
								}
								else
								{
									x = write(STDOUT_FILENO, "message not found\n", 19);
									chkErr(x, "write ouput");
								}
								isIP = true;
								break;
							}
						}
					}
					if (!isIP)
					{
						char message[100] = {};
						sprintf(message, "%s ", ptr);
						ptr = strtok(NULL, "");
						if (ptr != NULL)
						{
							strcat(message, ptr);
						}
						for (ClientHandler i : connList)
						{
							if (i.isActive)
							{
								int x = write(i.conn_write_fd, message, strlen(message)); //write message to conn handlers
								chkErr(x, "write message to conn handlers");
							}
						}
					}
				}
			}
			else
			{
				x = write(STDOUT_FILENO, "connection list is empty\n", 26);
				chkErr(x, "write ouput");
			}
		}
		else
		{
			write(STDOUT_FILENO, "Command is not recognized by CONN\n", 35);
		}
	}
	pthread_exit(NULL);
}

//thread function to communicate with client handler
void *client_handler_io_runner(void *fds)
{
	int r = ((struct RW *)fds)->read;
	int w = ((struct RW *)fds)->write;
	int m = ((struct RW *)fds)->msgsock;
	char buff_comm[50] = {};
	while (true)
	{
		//read command from conn
		int x = read(r, buff_comm, 50);
		if (x == 0)
			pthread_exit(NULL);
		chkErr(x, "read command from conn");
		char checkComm[5] = {};
		strncpy(checkComm, buff_comm, 4);
		checkComm[4] = '\0';
		if (strcmp(checkComm, "list") == 0)
		{
			char list[1000] = {};
			listProcess(list);
			int ret = write(w, list, strlen(list));
			chkErr(ret, "write list to conn");
		}
		else //print message
		{
			char message[100] = {};
			sprintf(message, "%s\n", buff_comm);
			//message[x] = '\0';
			int ret = write(m, message, strlen(message));

			chkErr(ret, "message to client");
			ret = write(STDOUT_FILENO, "Message sent\n", 13);
			chkErr(ret, "message confirmation");
		}
	}
}

int checkInput(char *input)
{
	int j = 0;
	while (input[j])
	{

		if (!isdigit(input[j]))
		{
			return 0;
		}
		j++;
	}
	return 1;
}

int checkDiv(char *input)
{
	int j = 0;
	while (input[j])
	{

		if (input[j] == '0')
		{
			return 0;
		}
		j++;
	}
	return 1;
}

void signal_handler(int signo)
{
	if (signo == SIGCHLD)
	{
		int status;
		child_pid = waitpid(0, &status, WNOHANG);

		if (child_pid != -1)
		{
			if (!allProcess.empty()) //if client processes close
			{
				list<Process>::iterator i;
				for (i = allProcess.begin(); i != allProcess.end(); i++)
				{
					Process p;
					p = *i;
					if (p.end_time == NULL) //active
					{
						if (p.pid == child_pid)
						{
							(*i).end_time = time(0);
							(*i).elapsed_time = difftime((*i).end_time, (*i).start_time);
							child_pid = -1;
							break;
						}
					}
				}
			}
			if (!connList.empty()) //if connection handler closes
			{
				list<ClientHandler>::iterator j;
				for (j = connList.begin(); j != connList.end(); j++)
				{
					ClientHandler ch;
					ch = *j;
					if (ch.isActive == true) //active
					{
						if (ch.ch_pid == child_pid)
						{
							(*j).isActive = false;
							child_pid = -1;
							break;
						}
					}
				}
			}
		}
	}
	else if (signo == SIGTERM)
	{
		if (!allProcess.empty()) // if client handler terminates then close clients process
		{
			list<Process>::iterator i;
			for (i = allProcess.begin(); i != allProcess.end(); i++)
			{
				Process p;
				p = *i;
				kill(p.pid, SIGTERM);
			}
		}
		exit(0);
	}
}

int main(void)
{
	int sock, length;
	struct sockaddr_in server;
	int msgsock;

	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("opening stream socket");
		exit(1);
	}

	int option = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	/* Name socket using wildcards */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(38377);
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("binding stream socket");
		exit(1);
	}

	write(STDOUT_FILENO, "Awaiting connection request...\n", 32);
	fflush(stdout);

	/* Establish conn I/O thread */
	pthread_t conn_io_thread;
	int rc = pthread_create(&conn_io_thread, NULL, conn_io_runner, NULL);
	if (rc)
	{
		perror("create connection I/O thread");
		exit(1);
	}

	/* Start accepting connections */
	listen(sock, 5);

	char buff_comm[1024];
	int rval;
	int x;

	//handle process termination from outside
	signal(SIGCHLD, signal_handler);
	signal(SIGTERM, signal_handler);
	while (TRUE)
	{
		struct sockaddr_in client;
		socklen_t l = sizeof(client);
		msgsock = accept(sock, (struct sockaddr *)&client, &l);
		if (msgsock == -1)
			perror("accept");
		else
		{
			int fd1[2];
			int fd2[2];

			int ret = pipe(fd1);
			chkErr(ret, "create pipe 1");
			ret = pipe(fd2);
			chkErr(ret, "create pipe 2");
			struct RW *fds = (struct RW *)malloc(sizeof(struct RW));
			fds->read = fd1[0];
			fds->write = fd2[1];
			fds->msgsock = msgsock;

			int pid = fork();
			chkErr(pid, "client handler fork");
			if (pid > 0) //server=conn
			{
				close(fd1[0]);
				close(fd2[1]);
				ClientHandler c;
				c.ch_pid = pid;
				sprintf(c.ip, "%s", inet_ntoa(client.sin_addr));
				c.conn_read_fd = fd2[0];
				c.conn_write_fd = fd1[1];

				c.isActive = true;
				connList.push_back(c);
			}
			else if (pid == 0) //client handler
			{
				close(fd1[1]);
				close(fd2[0]);
				/* Establish client handler I/O thread */
				pthread_t client_handler_io_thread;
				int rc = pthread_create(&client_handler_io_thread, NULL, client_handler_io_runner, (void *)fds);
				if (rc)
				{
					perror("create client handler I/O thread");
					exit(1);
				}
				do
				{
					bzero(buff_comm, sizeof(buff_comm));
					//read from client
					if ((rval = read(msgsock, buff_comm, 1024)) < 0)
						perror("read command");
					if (rval == 0)
					{

						if (!allProcess.empty())
						{
							list<Process>::iterator i;
							for (i = allProcess.begin(); i != allProcess.end(); i++)
							{
								Process p;
								p = *i;
								kill(p.pid, SIGTERM);
							}
						}
						write(STDOUT_FILENO, "connection ended with client\n", 30);
						exit(0);
					}
					else
					{

						buff_comm[rval - 1] = NULL;

						char *ptr = strtok(buff_comm, " ");
						if (ptr == NULL)
						{
							write(msgsock, "\n", 2);
							chkErr(x, "output to client");
						}
						else if (strcmp(buff_comm, "exit") == 0)
						{
							for (Process p : allProcess)
							{
								if (p.end_time == NULL) //active
								{
									int kill_stat = kill(p.pid, SIGTERM);
									chkErr(kill_stat, "exit running programs");
									int status;
									int x = waitpid(p.pid, &status, 0);
									chkErr(x, "wait err");
								}
							}
							write(msgsock, "exit", 5);
							chkErr(x, "output to client");
							write(STDOUT_FILENO, "connection ended with client\n", 30);
							exit(0);
						}
						else if (strcmp(ptr, "list") == 0)
						{
							ptr = strtok(NULL, " ");
							if (ptr == NULL) //list
							{
								char list[1000] = {};
								listProcess(list);
								int x = write(msgsock, list, strlen(list));
								chkErr(x, "write output list");
							}
							else if (strcmp(ptr, "all") == 0) //list all
							{
								char list[1000] = {};
								listAllProcess(list);
								int x = write(msgsock, list, strlen(list));
								chkErr(x, "write output list all");
							}
							else //err
							{
								write(msgsock, "Invalid command", 16);
								chkErr(x, "output to client");
							}
						}
						else if (strcmp(ptr, "run") == 0)
						{
							ptr = strtok(NULL, " ");
							if (ptr != NULL)
							{
								int pipe_status, exec_status, fork_status, pipe_descriptors[2];
								pipe_status = pipe2(pipe_descriptors, O_CLOEXEC);
								chkErr(pipe_status, "pipe of run");
								fork_status = fork();
								chkErr(fork_status, "fork of run");
								if (fork_status == 0)
								{
									int exec_status = execlp(ptr, ptr, NULL);
									if (exec_status == -1)
									{
										char error_buffer[4] = {0};
										int error = errno;
										sprintf(error_buffer, "%d", error);
										close(pipe_descriptors[0]);
										write(pipe_descriptors[1], error_buffer, sizeof(int));
										close(pipe_descriptors[1]);
										exit(0);
									}
								}
								if (fork_status > 0)
								{

									char buffer[4] = {0};
									close(pipe_descriptors[1]);
									int bytes_read = read(pipe_descriptors[0], buffer, sizeof(int));
									if (bytes_read == 0)
									{
										Process p;
										p.pid = fork_status;
										strcpy(p.pname, ptr);
										p.start_time = time(0);
										allProcess.push_back(p);
										int x = write(msgsock, "exec success\n", 14);
										chkErr(x, "output to client");
									}
									else if (bytes_read > 0)
									{
										int error = atoi(buffer);
										char *err = strerror(error);
										int x = write(msgsock, err, strlen(err));
										chkErr(x, "output to client");
									}
								}
							}
							else
							{
								//write to client
								write(msgsock, "Invalid Path\n", 14);
								chkErr(x, "output to client");
							}
						}
						else if (strcmp(ptr, "kill") == 0)
						{
							bool killed = false;
							ptr = strtok(NULL, " ");
							if (ptr != NULL)
							{
								list<Process>::iterator i;
								for (i = allProcess.begin(); i != allProcess.end(); i++)
								{
									Process p;
									p = *i;
									if (p.end_time == NULL)
									{
										if (p.pid == atoi(ptr) || strcmp(ptr, p.pname) == 0)
										{

											int status;
											int kill_status = kill(p.pid, SIGTERM);
											chkErr(kill_status, "Could not kill");
											if (kill_status == 0)
											{
												waitpid(p.pid, &status, 0);
												killed = true;
												(*i).end_time = time(0);
												(*i).elapsed_time = difftime((*i).end_time, (*i).start_time);
												write(msgsock, "\n", 1);
												break;
											}
										}
									}
								}
								if (!killed)
								{
									//write to client
									int x = write(msgsock, "Invalid pid or pname\n", 22);
									chkErr(x, "output to client");
								}
							}
							else
							{
								//write to client
								write(msgsock, "Invalid process id or name\n", 28);
								chkErr(x, "output to client");
							}
						}
						else if (strcmp(ptr, "add") == 0)
						{
							//var
							int sum = 0;
							char totalSum[10];
							bool valid = true;
							ptr = strtok(NULL, " ");
							if (ptr == NULL)
								valid = false;
							while (ptr != NULL)
							{
								if (checkInput(ptr) == 1)
								{
									int i = 0;
									sscanf(ptr, "%d", &i);
									sum += i;
									ptr = strtok(NULL, " ");
								}
								else
								{
									valid = false;
									break;
								}
							}
							if (valid)
							{
								int check = sprintf(totalSum, "%d\n", sum);
								chkErr(check, "convert sum");
								//write to client
								check = write(msgsock, totalSum, check);
								chkErr(check, "write sum to client");
							}
							else
							{
								//write to client
								write(msgsock, "Invalid Input\n", 15);
								chkErr(x, "output to client");
							}
						}
						else if (strcmp(ptr, "sub") == 0)
						{
							//var
							int sum;
							char totalSum[10];
							bool valid = true;
							ptr = strtok(NULL, " ");
							if (ptr == NULL)
								valid = false;
							if (ptr != NULL && checkInput(ptr) == 1)
							{
								int i = 0;
								sscanf(ptr, "%d", &i);
								sum = i;
								ptr = strtok(NULL, " ");
							}
							else
								valid = false;
							while (ptr != NULL && valid)
							{
								if (checkInput(ptr) == 1)
								{
									int i = 0;
									sscanf(ptr, "%d", &i);
									sum -= i;
									ptr = strtok(NULL, " ");
								}
								else
								{
									valid = false;
								}
							}
							if (valid)
							{
								int check = sprintf(totalSum, "%d\n", sum);
								chkErr(check, "convert sub");
								//write to client
								check = write(msgsock, totalSum, check);
								chkErr(check, "write subtraction to client");
							}
							else
							{
								//write to client
								write(msgsock, "Invalid Input\n", 15);
								chkErr(x, "output to client");
							}
						}
						else if (strcmp(ptr, "mul") == 0)
						{
							//var
							int prod = 1;
							char totalProd[10];
							bool valid = true;
							ptr = strtok(NULL, " ");
							if (ptr == NULL)
								valid = false;
							while (ptr != NULL && valid)
							{
								if (checkInput(ptr) == 1)
								{
									int i = 0;
									sscanf(ptr, "%d", &i);
									prod *= i;
									ptr = strtok(NULL, " ");
								}
								else
								{
									valid = false;
								}
							}
							if (valid)
							{
								int check = sprintf(totalProd, "%d\n", prod);
								chkErr(check, "convert product");
								//write to client
								check = write(msgsock, totalProd, check);
								chkErr(check, "write prod to client");
							}
							else
							{
								//write to client
								write(msgsock, "Invalid Input\n", 15);
								chkErr(x, "output to client");
							}
						}
						else if (strcmp(ptr, "div") == 0)
						{
							//var
							float div = 1;
							char totalDiv[10];
							bool valid = true;
							ptr = strtok(NULL, " ");
							if (ptr == NULL)
								valid = false;
							if (ptr != NULL && checkInput(ptr) == 1)
							{

								int i = 0;
								sscanf(ptr, "%d", &i);
								div = i;
								ptr = strtok(NULL, " ");
							}
							else
							{
								valid = false;
							}
							//printf(ptr);
							while (ptr != NULL && valid)
							{
								if (checkInput(ptr) == 1 && checkDiv(ptr) == 1)
								{
									int i = 0;
									sscanf(ptr, "%d", &i);
									div /= i;
									ptr = strtok(NULL, " ");
								}
								else
								{
									valid = false;
								}
							}
							if (valid)
							{
								int check = sprintf(totalDiv, "%.2f\n", div);
								chkErr(check, "convert div");
								//write to client
								check = write(msgsock, totalDiv, check);
								chkErr(check, "write div to client");
							}
							else
							{
								//write to client
								write(msgsock, "Invalid Input\n", 15);
								chkErr(x, "output to client");
							}
						}
						else
						{
							//write to client
							write(msgsock, "This Command is not valid\n", 27);
							chkErr(x, "output to client");
						}
					}
				} while (rval != 0);
			}
		}
		close(msgsock);
	}
}
