#include "session.hpp"

#include <functional>

#include "sessionManager.hpp"

namespace snesl
{
    Session::Session(const std::shared_ptr<SessionManager> parent, const asio::io_service& service) :
        _parent(parent), _socket(const_cast<asio::io_service&>(service))
    {
    }

    Session::~Session()
    {
        StopSession();
    }

    asio::ip::tcp::socket & Session::GetSocket()
    {
        return _socket;
    }

    void Session::ConnectionStarted(uint64_t id)
    {
        _sessionId = id;
        asio::async_read(_socket, asio::buffer(&_readSize, 4), std::bind(&Session::HandleReadSize, this, std::placeholders::_1, std::placeholders::_2));
    }

    void Session::HandleReadSize(const asio::error_code& /*error*/, const size_t& /*bytes*/)
    {
        asio::async_read(_socket, asio::buffer(&_readMsg, _readSize), std::bind(&Session::HandleReadMsg, this, std::placeholders::_1, std::placeholders::_2));
    }

    void Session::HandleReadMsg(const asio::error_code& /*error*/, const size_t& /*bytes*/)
    {
        asio::async_read(_socket, asio::buffer(&_readSize, 4), std::bind(&Session::HandleReadSize, this, std::placeholders::_1, std::placeholders::_2));
    }

    void Session::HandleWrite(const asio::error_code& /*error*/, size_t /*bytes_transferred*/)
    {
        _writeMsgs.pop();
        SendMessage();
    }

    void Session::AddMessageToSend(std::string msg)
    {
        _writeMutex.lock();
        _writeMsgs.push(msg);
        _writeMutex.unlock();

        if (_writeMsgs.size() == 1)
            SendMessage();
    }

    void Session::SendMessage()
    {
        std::lock_guard<std::mutex> guard(_writeMutex);

        if (_writeMsgs.empty())
            return;

        auto msg = _writeMsgs.front();
        asio::async_write(_socket, asio::buffer(msg), std::bind(&Session::HandleWrite, this, std::placeholders::_1, std::placeholders::_2));
    }

    uint64_t Session::GetSessionId()
    {
        return _sessionId;
    }

    void Session::StopSession()
    {
        _stopping = true;
        asio::error_code ec;

#ifndef WIN32
        _socket.cancel(ec);
#endif

        _socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        _socket.close();
    }
}
