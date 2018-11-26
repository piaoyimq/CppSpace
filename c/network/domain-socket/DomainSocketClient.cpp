

#include "DomainSocketClient.h"




DomainSocketClient::DomainSocketClient(const std::string& serverSocketFile, MsgType msgType, const std::string& clientSocketFile) :
        _serverSocketFile(serverSocketFile), _msgType(msgType), _clientSocketFile(clientSocketFile)
{
    _setLogger();
}


void DomainSocketClient::_setLogger()
{
    if (!_logger.get())
    {
        _logger.reset(new Poco::LogStream(pu::Application::instance().logger()));
        poco_check_ptr(_logger);
    }
}


bool DomainSocketClient::send(const std::string& data)
{
    const std::string wrapperData(getMsgTypeString(_msgType) + data);
    std::string::size_type wrapperDataSize = wrapperData.size();

    struct sockaddr_un clientAddr, serverAddr;

    if ((_sockFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("client socket error");
        return false;
    }

    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sun_family = AF_UNIX;
    strcpy(clientAddr.sun_path, _clientSocketFile.c_str());
    size_t len = offsetof(struct sockaddr_un, sun_path) + strlen(clientAddr.sun_path);

    unlink(_clientSocketFile.c_str());

    if (bind(_sockFd, (struct sockaddr *) &clientAddr, len) < 0)
    {
        perror("bind error");
        return false;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, _serverSocketFile.c_str());
    len = offsetof(struct sockaddr_un, sun_path) + strlen(serverAddr.sun_path);

    if (connect(_sockFd, (struct sockaddr *) &serverAddr, len) < 0)
    {
        perror("connect error");
        return false;
    }

    int pos = 0, n = 0;
    int segmentSize = 1500;

    while (pos < wrapperDataSize)
    {
        std::string segment = wrapperData.substr(pos, segmentSize);
        pos += segmentSize;
        n = write(_sockFd, segment.c_str(), segment.size());

        if (segment.size() != n)
        {
            _logger->error() << "Send to pop up window failed" << std::endl;
            return false;
        }
    }

    return true;
}
