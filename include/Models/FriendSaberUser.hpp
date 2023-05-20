#pragma once
#include <string>
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "bsml/shared/BSML/Components/CustomListTableData.hpp"
#include "main.hpp"

namespace FriendSaber::Models{
    struct FriendSaberUser{
        unsigned long _playerId;
        unsigned int _rank;
        std::string _playerName;
        std::string _imageUrl;
        UnityEngine::Sprite* _profilePicture = nullptr;

        FriendSaberUser(unsigned long playerId, unsigned int rank, std::string playerName, std::string imageUrl, UnityEngine::Sprite* profilePicture) :
        _playerId(playerId), _rank(rank), _playerName(playerName), _imageUrl(imageUrl), _profilePicture(profilePicture) {}

        ~FriendSaberUser(){
            if (_profilePicture == nullptr || _profilePicture->m_CachedPtr.m_value == nullptr) return;
            if (_profilePicture->get_texture() == nullptr || _profilePicture->get_texture()->m_CachedPtr.m_value == nullptr) {
                UnityEngine::Object::Destroy(_profilePicture); return;
            }
            UnityEngine::Object::Destroy(_profilePicture->get_texture());
            UnityEngine::Object::Destroy(_profilePicture);
        }
    };
}