#include "sessionManager.hpp"

#include "session.hpp"

namespace snesl
{
    SessionManager::SessionManager()
    {
    }

    SessionManager::~SessionManager()
    {
    }

    std::shared_ptr<Session> SessionManager::CreateSession(const asio::io_service & service)
    {
        std::shared_ptr<Session> tmpSession(new Session(shared_from_this(), service));

        return tmpSession;
    }

    void SessionManager::StartSession(const std::shared_ptr<Session> & session)
    {
        std::lock_guard<std::mutex> guard(_sessionLocker);

        session->ConnectionStarted(++_maxSessionId);

        _sessions.push_back(session);
    }
}
