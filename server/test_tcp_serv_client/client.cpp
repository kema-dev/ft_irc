#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

int main (int argc, char* argv[])
{
    int listenFd, portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;

    if(argc != 3)
    {
        cerr<<"Usage : ./client <host name> <port>"<<endl;
        return 0;
    }

    portNo = atoi(argv[2]);

    if((portNo > 65535) || (portNo < 2000))
    {
        cerr<<"Please enter port number between 2000 - 65535"<<endl;
        return 0;
    }       

    //create client skt
    listenFd = socket(PF_INET, SOCK_STREAM, 0);

    if(listenFd < 0)
    {   
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    server = gethostbyname(argv[1]);

    if(server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }

    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;

    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);

    svrAdd.sin_port = htons(portNo);

    int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));

    if (checker < 0)
    {
        cerr << "Cannot connect!" << endl;
        return 0;
    }

    //send stuff to server
    for(;;)
    {
        char s[300];
        //cin.clear();
        //cin.ignore(256, '\n');
        cout << "Enter stuff: ";
        bzero(s, 300);
        cin.getline(s, 300);

        write(listenFd, s, strlen(s));
    }
}
