#pragma once
#include <string>

namespace FriendSaber::Utils::Constants{
    static const std::string API_URL = "https://scoresaber.com/api";
    static const std::string PLAYER_SEARCH = "/players?search=";
    static const std::string PLAYER_ID_SEARCH = "/player/";
    static const std::string BASIC = "/basic";
    static const std::string FULL = "/full";

    static const std::string DATA_DIR = "/sdcard/ModData/com.beatgames.beatsaber/Mods/ScoreSaber";
    static const std::string STEAM_KEY_PATH = DATA_DIR + "/scoresaber_DO_NOT_SHARE.scary";
    static const std::string FRIENDS_PATH = DATA_DIR + "/friends.txt";
}