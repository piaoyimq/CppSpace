
#ifndef DomainServer_H_
#define DomainServer_H_


class DomainServer
{
public:
    DomainServer(const std::string& socketFile):_socketFile(socketFile)
    {
    }

    ~DomainServer()
    {
        close(_listenFd);
        unlink(_socketFile.c_str());
    }

    void start(void*(*handleCallback)(void*));

    int getConnectionFd();

private:

    int _listenFd;

    int _connFd;

    std::string _socketFile;
};

#endif
