#pragma once

#include <memory>
#include <mutex>
#include <vector>

namespace asio
{
    class io_service;
}

namespace snesl
{
    class Session;

    class SessionManager : public std::enable_shared_from_this < SessionManager >
    {
    public:
        SessionManager();
        ~SessionManager();
        SessionManager(const SessionManager&) = delete;
        SessionManager operator = (const SessionManager&) = delete;

        std::shared_ptr<Session> CreateSession(const asio::io_service & service);

        void StartSession(const std::shared_ptr<Session> & session);

    private:
        std::vector<std::shared_ptr<Session> > _sessions;
        std::mutex _sessionLocker;

        uint64_t _maxSessionId;
    };
}
