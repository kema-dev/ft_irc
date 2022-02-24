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
    int pId, portNo, listenFd;
    socklen_t len;
    bool loop = false;
	struct sockaddr_in svrAdd, clntAdd;

	pthread_t threadA[3];

	if (argc < 2)
	{
		cerr << "Syntam : ./server <port>" << endl;
		return 0;
	}

	portNo = atoi(argv[1]);

	if((portNo > 65535) || (portNo < 2000))
	{
		cerr << "Please enter a port number between 2000 - 65535" << endl;
		return 0;
	}

	//create socket
	listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(listenFd < 0)
	{
		cerr << "Cannot open socket" << endl;
		return 0;
	}

	bzero((char*) &svrAdd, sizeof(svrAdd));

	svrAdd.sin_family = AF_INET;
	svrAdd.sin_addr.s_addr = INADDR_ANY;
	svrAdd.sin_port = htons(portNo);

	//bind socket
	if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
	{
		cerr << "Cannot bind" << endl;
		return 0;
	}

    listen(listenFd, 5);

    int noThread = 0;

    while (noThread < 3)
    {
        socklen_t len = sizeof(clntAdd);
        cout << "Listening" << endl;

    //this is where client connects. svr will hang in this mode until           client conn
        connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd < 0)
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        }
        else   
        {
            cout << "Connection successful" << endl;
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
    cout << "Thread No: " << pthread_self() << endl;
    char test[256];
    bzero(test, 256);
    bool loop = false;
    while(!loop)
    {       
        bzero(test, 256);     
        int n = read(connFd, test, 255);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",test);            
    }
    cout << "\nClosing thread and conn" << endl;
    close(connFd);
	return (NULL);
}
