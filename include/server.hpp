#pragma once

#include <memory>
#include <mutex>
#include <list>
#include <string>
#include <thread>

#include <asio.hpp>

#include "sessionManager.hpp"

namespace snesl
{
    // TODO: ipv6 support
    enum ServerRunMode
    {
        IPv4Only,
        IPv6Only,
        Mixed
    };

    class Server
    {
    public:
        Server();
        Server(std::string, uint32_t);

        Server(Server&) = delete;
        Server(Server&&) = delete;

        Server& operator =(Server&) = delete;

        void Bind(std::string, uint32_t);

        void Run();

        void Stop();
        void StopAcceptor();

        uint32_t GetListenPort();
        std::string GetListenAddress();

        uint8_t GetConnectorsCount();
        void SetConnectorsCount(uint8_t);

    private:
        std::string _listenAddress;
        uint32_t _listenPort;
        uint8_t _connectorsCount;

        std::mutex _connectorsMutex;

        std::list<std::thread> _connectors;

        asio::io_service _service;
        asio::ip::tcp::acceptor _acceptor;

        std::shared_ptr<SessionManager> _sessionManager;

        void AcceptNewConnection();
        void HandleNewConnection(std::shared_ptr<Session>, const asio::error_code&);

        void Unbind();
    };
}
