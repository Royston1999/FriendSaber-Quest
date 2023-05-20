#pragma once
#include <string>
#include <functional>

namespace WebUtils{
    extern std::string cookie;
}
namespace ScoreSaber::Services::PlayerService
{
    enum LoginStatus
    {
        Error,
        Success,
    };
    enum AuthState
    {
        NotStarted,
        Started,
        Finished
    };
    extern enum AuthState authState;
    extern void AuthenticateUser(std::function<void(LoginStatus)>);
}
