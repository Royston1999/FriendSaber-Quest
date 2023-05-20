#pragma once

#include <string>
#include <functional>
#include "Models/FriendSaberUser.hpp"
#include <optional>

namespace FriendSaber::Downloaders {
    void GetPlayersByString(std::string searchQuery, std::function<void(std::optional<std::vector<Models::FriendSaberUser>>)> callback);
    void GetPlayerById(std::string playerId, std::function<void(std::optional<Models::FriendSaberUser>)> callback);
}