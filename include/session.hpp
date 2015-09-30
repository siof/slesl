#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include <asio.hpp>

#ifdef WIN32
#undef SendMessage
#endif

namespace snesl
{
    class SessionManager;

    class Session : public std::enable_shared_from_this < Session >
    {
    public:
        Session(const std::shared_ptr<SessionManager> parent, const asio::io_service&);
        ~Session();

        asio::ip::tcp::socket & GetSocket();

        void ConnectionStarted(uint64_t id);

        void HandleReadSize(const asio::error_code& error, const size_t& /*bytes*/);
        void HandleReadMsg(const asio::error_code& error, const size_t& /*bytes*/);

        void HandleWrite(const asio::error_code& error, size_t /*bytes_transferred*/);

        void AddMessageToSend(std::string msg);
        void SendMessage();

        uint64_t GetSessionId();

        void StopSession();

    private:
        std::mutex _writeMutex;

        asio::ip::tcp::socket _socket;
        uint32_t _readSize;
        std::vector<char> _readMsg;
        std::queue<std::string> _writeMsgs;
        uint64_t _sessionId;

        std::shared_ptr<SessionManager> _parent;

        bool _stopping;
    };
}
