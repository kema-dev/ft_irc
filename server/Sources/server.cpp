#include <iostream>
#include "../Classes/server.hpp"
#include "../Classes/error.hpp"
#include "../Classes/commandHandler.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

void*	task1(void *);

static int connFd;
void	error(const char* msg)
{
	perror(msg);
	exit(1);
}

int	main(int argc, char** argv)
{
	try {
		if (argc != 3)
			throw(BadNumberArgs());
	}
	catch(const BadNumberArgs e) {
		std::cerr << e.info() << std::endl;
		return (BAD_NUMBER_ARGS);
	}
	try {
		if (std::atoi(argv[1]) < 1 || std::atoi(argv[1]) > 65535)
			throw(InvalidPort());
	}
	catch (const InvalidPort e) {
		std::cerr << e.info() << std::endl;
		return (INVALID_PORT);
	}

	int portNo, listenFd;
	struct sockaddr_in svrAdd, clntAdd;

	pthread_t threadA[3];
	portNo = atoi(argv[1]);

	//create socket
	listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	try {
		if(listenFd < 0)
			throw (ErrorInSocketCreation());
	}
	catch (const ErrorInSocketCreation e) {
		std::cerr << e.info() << std::endl;
		return (ERR_SOCKET_CREATION);
	}
	
	bzero((char*) &svrAdd, sizeof(svrAdd));

	svrAdd.sin_family = AF_INET;
	svrAdd.sin_addr.s_addr = INADDR_ANY;
	svrAdd.sin_port = htons(portNo);

	//bind socket
	try {
		if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
			throw (ErrorInBinding());
	}
	catch (const ErrorInBinding e) {
		std::cerr << e.info() << std::endl;
		return (ERR_BIND);
	}
	

	listen(listenFd, 5);

	int noThread = 0;

	while (noThread < 3)
	{
		socklen_t len = sizeof(clntAdd);
		cout << "Listening to port " << argv[1] << endl;

	//this is where client connects. svr will hang in this mode until		   client conn
		connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

		try {
			if (connFd < 0)
				throw (CannotAcceptConnection());
			else   
				cout << "Connection established." << endl;
		}
		catch (const CannotAcceptConnection e){
			std::cerr << e.info() << std::endl;
			return (ERR_CONNECTION);
		}

		pthread_create(&threadA[noThread], NULL, task1, NULL); 
		noThread++;
	}

	for(int i = 0; i < 3; i++)
	{
		pthread_join(threadA[i], NULL);
	}  
}

void *task1 (void *dummyPt)
{
	static_cast<void>(dummyPt);
	cout << "Thread No: " << pthread_self() << endl;
	char test[256];
	bzero(test, 256);
	bool loop = false;
	while(!loop)
	{	   
		bzero(test, 256);	 
		int n = read(connFd, test, 255);
		if (n < 0) error("ERROR reading from socket");
		if (command_check(test) < 0)
			std::cout << "Invalid command" << std::endl;
		printf("Here is the message: %s\n",test);			
	}
	cout << "\nClosing thread and conn" << endl;
	close(connFd);
	return (NULL);
}
