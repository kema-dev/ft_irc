#include "../Error/Error.hpp"
#include "../CommandHandler/CommandHandler.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

typedef struct s_params
{
    int fd;
}   t_params;

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

	vector<pthread_t> threadV;
	portNo = atoi(argv[1]);

	//create socket
	listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    try {
        int enable = 1;
        if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            throw(ErrorReusingSocket());
    }
    catch (const ErrorReusingSocket e) {
        std::cerr << e.info() << std::endl;
    }
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
    t_params params;
    std::cout << listenFd << std::endl;

    socklen_t len = sizeof(clntAdd);
    while(1)
    {
        try {
            cout << "Listening to port " << argv[1] << endl;
            connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);
            if (connFd < 0)
                throw (CannotAcceptConnection());
            else  
            {    
                pthread_t  a;
                threadV.push_back(a);
                params.fd = connFd;
                std::cout << "connFd: " << connFd << std::endl;
                pthread_create(&threadV.back(), NULL, task1, &params); 
                cout << "Connection established." << endl;
            }
        }
        catch (const CannotAcceptConnection e){
            std::cerr << e.info() << std::endl;
            return (ERR_CONNECTION);
        }
    }
}

void *task1 (void *dummyPt)
{
    t_params* params = static_cast<t_params*>(dummyPt);
    ssize_t i;
    cout << "-----------------" << endl;
	//cout << "Thread No: " << pthread_self() << endl;
	char test[256];
	bzero(test, 256);
	bool loop = false;
    i = 0;
    send(params->fd, "Connection established\n", strlen("Connection established\n"), MSG_DONTWAIT);
	while(!loop)
	{
		bzero(test, 256);	 
		int n = read(connFd, test, 255);
        try {
            if (n < 0)
                throw(ReadImpossible());
        }
        catch (const ReadImpossible e){
            std::cerr << e.info() << std::endl;
        }
        // cout << "Thread No: " << pthread_self() <<  ":" << i << endl;
		cout << "Input:" << test << endl;
		if (i > 2)
        {
            try{
                if (command_check(test, params->fd) < 0)
                    throw(NotACommand());
            }
            catch (const NotACommand e) {
                std::cerr << e.info() << std::endl;
            }
        }
        i++;
	}
	cout << "\nClosing thread and conn" << endl;
	close(connFd);
	return (NULL);
}
