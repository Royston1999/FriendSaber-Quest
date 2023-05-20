#include "UI/CustomList/FriendSaberListCell.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "Utils/WebRequest.hpp"

#include "assets.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/Helpers/getters.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "Utils/StringUtils.hpp"

DEFINE_TYPE(FriendSaber::CustomList, FriendSaberListCell);

using namespace UnityEngine;
using namespace UnityEngine::UI;

UnityEngine::Color highlightedColor = UnityEngine::Color(0.0f, 0.71f, 0.82f, 0.8f);
UnityEngine::Color idleColor = UnityEngine::Color(0.0f, 0.0f, 0.0f, 0.8f);

namespace FriendSaber::CustomList
{
    FriendSaber::CustomList::FriendSaberListCell* FriendSaberListCell::CreateNewCell()
    {
        static ConstString playerTableCellStr("FriendSaberListCell");
        auto cellGO = UnityEngine::GameObject::New_ctor();
        auto playerCell = cellGO->AddComponent<FriendSaberListCell*>();
        cellGO->set_name(playerTableCellStr);

        BSML::parse_and_construct(IncludedAssets::FriendSaberListCell_bsml, cellGO->get_transform(), playerCell);
        return playerCell;
    }

    void FriendSaberListCell::PostParse()
    {
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        backgroundImage->background->skew = 0.0f;
        backgroundImage->background->set_color0(idleColor);
        backgroundImage->background->set_color1(idleColor);
    }

    void FriendSaberListCell::Populate(Models::FriendSaberUser& user){
        _user = &user;
        set_name(_user->_playerName);
        set_sub(string_format("#%i", user._rank));
        set_sync_active();

        if (_user->_playerId == tableData->localPlayerId && tableData->globalFriendsList.contains(std::to_string(_user->_playerId))){
            set_button_text("remove");
            toggle_friend->set_interactable(false);
        }
        else {
            std::string stringified = std::to_string(_user->_playerId);
            tableData->currentListEdits.contains(stringified) ? set_button_text("remove") : set_button_text("add");
            toggle_friend->set_interactable(true);
        }

        if (_user->_profilePicture != nullptr && _user->_profilePicture->m_CachedPtr.m_value != nullptr
            && _user->_profilePicture->get_texture() != nullptr && _user->_profilePicture->get_texture()->m_CachedPtr.m_value != nullptr) return set_sprite(_user->_profilePicture);

        downloadInProgress = true;
        Utils::SendImageRequest(_user->_imageUrl, [this](UnityEngine::Sprite* profilePicture){
            _user->_profilePicture = profilePicture;
            set_sprite(profilePicture);
            downloadInProgress = false;
        });
    }

    void FriendSaberListCell::HighlightDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        backgroundImage->background->set_color1(get_highlighted() ? highlightedColor : idleColor);
        backgroundImage->background->set_color(get_highlighted() ? highlightedColor : idleColor);
    }

    void FriendSaberListCell::ToggleFriend()
    {
        if (tableData->get_name() == "FriendsList") return tableData->deleteFriend(_user);
        
        getLogger().info("you have clicked on %s's profile!", _user->_playerName.c_str());
        std::string stringified = std::to_string(_user->_playerId);
        bool isFriended = tableData->currentListEdits.contains(stringified);
        if (isFriended) tableData->currentListEdits.erase(stringified);
        else tableData->currentListEdits.insert(stringified);
        set_sync_active();
        set_button_text(isFriended ? "add" : "remove");
        tableData->updateSyncability();
    }

    void FriendSaberListCell::FixedUpdate(){
        if (!get_gameObject()->get_activeInHierarchy() || downloadInProgress) return;

        if (_user->_profilePicture != nullptr && _user->_profilePicture->m_CachedPtr.m_value != nullptr
            && _user->_profilePicture->get_texture() != nullptr && _user->_profilePicture->get_texture()->m_CachedPtr.m_value != nullptr) return;
        downloadInProgress = true;
        Utils::SendImageRequest(_user->_imageUrl, [this](UnityEngine::Sprite* profilePicture){
            _user->_profilePicture = profilePicture;
            set_sprite(profilePicture);
            downloadInProgress = false;
        });
    }

    void FriendSaberListCell::set_name(std::string_view nameText)
    {
        name->SetText(nameText);
    }

    void FriendSaberListCell::set_sub(std::string_view subText)
    {
        sub->SetText(subText);
    }

    void FriendSaberListCell::set_sprite(UnityEngine::Sprite* sprite)
    {
        image->set_sprite(sprite);
    }

    void FriendSaberListCell::set_button_text(std::string text)
    {
        toggle_friend->get_transform()->GetComponentInChildren<TMPro::TextMeshProUGUI*>()->SetText(text);
    }

    void FriendSaberListCell::set_sync_active()
    {
        std::string stringified = std::to_string(_user->_playerId);
        bool inGlobal = tableData->globalFriendsList.contains(stringified);
        bool inLocal = tableData->currentListEdits.contains(stringified);
        syncWarning->get_gameObject()->set_active(inLocal != inGlobal);
    }
}