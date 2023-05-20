#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "bsml/shared/BSML/Components/CustomListTableData.hpp"
#include "HMUI/TableView.hpp"
#include "bsml/shared/BSML/Components/Settings/StringSetting.hpp"
#include "Models/FriendSaberUser.hpp"
#include "UI/CustomList/FriendSaberListData.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "HMUI/ModalView.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "HMUI/SegmentedControl.hpp"

DECLARE_CLASS_CODEGEN(FriendSaber, FriendSaberViewController, HMUI::ViewController, 
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    
    DECLARE_INSTANCE_FIELD(BSML::CustomListTableData*, searchResults);
    DECLARE_INSTANCE_FIELD(BSML::CustomListTableData*, friendsList);

    DECLARE_INSTANCE_FIELD(FriendSaber::CustomList::FriendSaberListData*, customSearchResults);
    DECLARE_INSTANCE_FIELD(FriendSaber::CustomList::FriendSaberListData*, customFriendsList);

    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, searchLoading);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, searchError);

    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, friendsLoading);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, friendError);

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, revertButton);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, syncButton);

    DECLARE_INSTANCE_FIELD(BSML::StringSetting*, searchBar);

    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, friendSaberModal);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, syncProgress);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, warningText);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, confirmButtonsLayout);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, closeButtonLayout);

    DECLARE_INSTANCE_METHOD(void, OnInput, StringW input);
    DECLARE_INSTANCE_METHOD(void, OnSearch);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, OpenSyncModal);
    DECLARE_INSTANCE_METHOD(void, OpenRevertModal);
    DECLARE_INSTANCE_METHOD(void, OnTabSelect, HMUI::SegmentedControl* tab, int index);
    DECLARE_INSTANCE_METHOD(void, FriendsListSearch);

    DECLARE_INSTANCE_METHOD(void, ConfirmModal);
    DECLARE_INSTANCE_METHOD(void, Cancel);


    private:
        bool friendsReloadScheduled;
        bool friendsFirstActivation;
        std::string searchQuery;
        std::string openModalID;
        std::string selectedTab;
        std::vector<Models::FriendSaberUser> searchResultsData;
        std::vector<Models::FriendSaberUser> friendsListData;
        void SetSearchLoading(UnityEngine::GameObject* loader, TMPro::TextMeshProUGUI* text, bool value, std::string error = "");
        void UpdateSyncability();
        void OnFriendsTabActivate(bool firstActivation, bool reloadScheduled);
        void OpenDeleteModal(Models::FriendSaberUser* player);
        int completionAmount;
)
