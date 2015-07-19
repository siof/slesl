#include "server.hpp"

#include <functional>

#include "session.hpp"

namespace snesl
{
    Server::Server() :
        _service(), _acceptor(_service), _connectorsCount(1), _sessionManager(new SessionManager())
    {
    }

    Server::Server(std::string ipAddress, uint32_t port) :
        Server()
    {
        Bind(ipAddress, port);
    }

    void Server::Bind()
    {
        asio::ip::tcp::resolver resolver(_service);
        asio::ip::tcp::resolver::query query(_listenAddress, std::to_string(_listenPort));
        asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

        _acceptor.open(endpoint.protocol());
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(endpoint);
        _acceptor.listen(/*asio::socket_base::max_connections*/);
    }

    void Server::Bind(std::string address, uint32_t port)
    {
        _listenAddress = address;
        _listenPort = port;

        Bind();
    }

    void Server::Run()
    {
        AcceptNewConnection();

        for (auto i = 0; i < _connectorsCount; ++i)
            _connectors.push_back(std::thread([this] { _service.run(); }));
    }

    void Server::Run(std::string ipAddress, uint32_t port)
    {
        Bind(ipAddress, port);
        Run();
    }

    void Server::Stop()
    {
        asio::error_code ec;
        _acceptor.cancel();
        _acceptor.close(ec);
        _service.stop();
    }

    uint32_t Server::GetListenPort()
    {
        return _acceptor.local_endpoint().port();
    }

    std::string Server::GetListenAddress()
    {
        return _acceptor.local_endpoint().address().to_string();
    }

    uint8_t Server::GetConnectorsCount()
    {
        return _connectorsCount;
    }

    void Server::SetConnectorsCount(uint8_t count)
    {
        _connectorsCount = count;
    }

    void Server::AcceptNewConnection()
    {
        std::shared_ptr<Session> newSession = _sessionManager->CreateSession(_service);
        _acceptor.async_accept(newSession->GetSocket(), std::bind(&Server::HandleNewConnection, this, newSession, std::placeholders::_1));
    }

    void Server::HandleNewConnection(std::shared_ptr<Session> session, const asio::error_code & error)
    {
        if (error)
        {
            //std::cout << "Error ( " << error.value() << " ) '" << error.message() << "' occured when accepting new session ... server will be stopped ..." << std::endl;
            Stop();
            return;
        }

        _sessionManager->StartSession(session);
        AcceptNewConnection();
    }
}
