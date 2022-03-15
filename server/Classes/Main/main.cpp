#include "../Error/Error.hpp"
#include "../CommandHandler/CommandHandler.hpp"
#include "../ServerManip/ServerManip.hpp"
#include "../Server/Server.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/event.h>

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

    socklen_t len = sizeof(clntAdd);
    //TODO Add try/catch
    Server* irc_serv = new Server("IRC_SERVER", argv[2]);
    int kq, new_event;
    struct kevent event_list[1];
    init_kqueue(listenFd, kq);
    cout << "Listening to port " << argv[1] << endl;
    while(1)
    {
        try {
            if ((new_event = kevent(kq, NULL, 0, event_list, 1, NULL)) == -1)
                throw (ErrKEvent());
        }
        catch (const ErrKEvent e) {
            std::cerr << e.info() << std::endl;
            exit(KEVENT_ERR);
        }
        int event_fd = event_list[0].ident;
        try {
            if ((connFd = accept(event_fd, (struct sockaddr *) &clntAdd, (socklen_t *) &len)) == -1)
                throw (CannotAcceptConnection());
            else
            {
                pthread_t  a;
                threadV.push_back(a);
                params.client_socket = connFd;
                params.irc_serv = irc_serv;
                pthread_create(&threadV.back(), NULL, task1, &params); 
                std::cout << "Connection established." << std::endl;
            }
        }
        catch (const CannotAcceptConnection e){
            std::cerr << e.info() << std::endl;
            return (ERR_CONNECTION);
        }   
    }
}

string read_socket(int socket)
{
    char input[256];
    static string buf;
    string ret;
    string input_s;

    bzero(input, 256);
    int n = recv(socket, input, 255, MSG_DONTWAIT);
    try {
        if (n < 0)
            throw(ReadImpossible());
    }
    catch (const ReadImpossible e){
        // std::cerr << e.info() << std::endl;
    }
    // std::cout << "Result from read:'" << input << "'" << std::endl;
    input_s = input;
    if (buf.empty() == 0)
        input_s = buf.append(input_s);
    ret = input_s.substr(0, input_s.find("\r"));
    buf = input_s.substr(input_s.find("\n") + 1, input_s.length() - (ret.length() + 1));
    return (ret);
}

void *task1 (void *dummyPt)
{
    t_params* params = reinterpret_cast<t_params*>(dummyPt);
    string nickname;
	string input;
    ssize_t id;
    size_t nbPass = 0;
    int security = 0;
    int nbError = 0;
	bool loop = false;

    // // ANCHOR xchat check
    // if (input_s.find("CAP") != std::string::npos)
    //     input_s.erase(0, strlen("CAP LS\n\r"));

    cout << "-----------------" << endl;
	while(!loop)
	{
        while ((input = read_socket(params->client_socket)).empty() == true)
        {
            ;
        }
        cout << "Input = '" << input << "'" << endl;
        // TODO add try/catch
        if (nbPass == 0)
        {
            if (check_password(input, params->irc_serv, params->client_socket) == 0)
                nbPass++;
            continue;
        }
        if ((nbPass == 1) || (nbPass == 2))
        {
            if (nickname.empty() == true)
            {
                nickname = parseNickname(input);
                nbPass++;
            }
            else
            {
                id = createUser(input, params->irc_serv, params->client_socket, nickname);
                params->user_id = id;
                nbPass++;
            }
        }
        if (nbPass == 3)
        {
            send(params->client_socket, ":localhost 001 dOD : Welcome to the Internet Relay Network dOD\r\n", strlen(":localhost 001 dOD : Welcome to the Internet Relay Network dOD\r\n"), 0);
            send(params->client_socket, ":localhost 002 dOD : Your host is localhost, running on version [42.42]\r\n", strlen(":localhost 002 dOD : Your host is localhost, running on version [42.42]\r\n"), 0);
            send(params->client_socket, ":localhost 003 dOD : This server was created Mon Mar 14 13:08:31 2022\r\n", strlen(":localhost 003 dOD : This server was created Mon Mar 14 13:08:31 2022\r\n"), 0);
            send(params->client_socket, ":localhost 004 dOD : localhost version [42.42]. Available user MODE : +Oa . Avalaible channel MODE : none.\r\n", strlen(":localhost 004 dOD : localhost version [42.42]. Available user MODE : +Oa . Avalaible channel MODE : none.\r\n"), 0);
            nbPass++;
            continue;
        }
        if (nbPass > 3)
        {
            try{
                security = command_check(input, params);
                if (security == CLIENT_DISCONNECTED)
                    nbError++;
                if (nbError >= 5)
                    throw (ClientDisconnected());
            }
            catch (const ClientDisconnected e) {
                std::cerr << e.info() << std::endl;
                close(params->client_socket);
                exit(CLIENT_DISCONNECTED);
            }
        }
	}
	cout << "\nClosing thread and connection." << endl;
	close(params->client_socket);
	return (NULL);
}
