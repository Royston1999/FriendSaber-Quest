#include "Downloaders/PlayerDownloader.hpp"
#include "Utils/Constants.hpp"
#include "Utils/WebRequest.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include <stdlib.h>

namespace FriendSaber::Downloaders {

    using namespace FriendSaber::Utils::Constants;
    using namespace rapidjson;

    template<class T>
    std::optional<T> GetValue(rapidjson::GenericObject<false, rapidjson::Value>& object, std::string name){
        auto itr = object.FindMember(name);
        if (itr != object.MemberEnd() && !itr->value.IsNull()) return std::optional<T>(itr->value.Get<T>());
        else return {};
    }

    void GetPlayersByString(std::string searchQuery, std::function<void(std::optional<std::vector<Models::FriendSaberUser>>)> callback){
        std::string url = API_URL + PLAYER_SEARCH + searchQuery;

        Utils::SendWebRequest(url, [callback](std::string response){
            std::vector<Models::FriendSaberUser> friends;
            Document document;
            document.Parse(response.c_str());
            if (document.HasMember("errorMessage") || document.Empty()) return callback({});
            auto players = document.FindMember("players")->value.GetArray();
            for (auto& entry : players){
                auto player = entry.GetObject();

                unsigned long playerId = std::stoul(GetValue<std::string>(player, "id").value_or("0"));
                std::string playerName = GetValue<std::string>(player, "name").value_or("Billie Oolie");
                unsigned int rank = GetValue<int>(player, "rank").value_or(0);
                std::string profileUrl = GetValue<std::string>(player, "profilePicture").value_or("null");
                
                friends.emplace_back(playerId, rank, playerName, profileUrl, nullptr);
            }
            callback(friends);
        });
    }

    void GetPlayerById(std::string playerId, std::function<void(std::optional<Models::FriendSaberUser>)> callback){
        std::string url = API_URL + PLAYER_ID_SEARCH + playerId + BASIC;

        Utils::SendWebRequest(url, [callback](std::string response){
            Document document;
            document.Parse(response.c_str());
            if (document.HasMember("errorMessage") || document.Empty()) return callback({});
            auto player = document.GetObject();

            unsigned long playerId = std::stoul(GetValue<std::string>(player, "id").value_or("0"));
            std::string playerName = GetValue<std::string>(player, "name").value_or("Billie Oolie");
            unsigned int rank = GetValue<int>(player, "rank").value_or(0);
            std::string profileUrl = GetValue<std::string>(player, "profilePicture").value_or("null");

            callback(Models::FriendSaberUser(playerId, rank, playerName, profileUrl, nullptr));
        });
    }
}