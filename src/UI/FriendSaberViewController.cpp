#include "UI/FriendSaberViewController.hpp"
#include "bsml/shared/BSML.hpp"
#include "assets.hpp"
#include "Downloaders/PlayerDownloader.hpp"
#include "Utils/WebRequest.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/VerticalScrollIndicator.hpp"
#include "Utils/ScoreSaberAccess.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/Constants.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"

DEFINE_TYPE(FriendSaber, FriendSaberViewController);

namespace FriendSaber{

    void FriendSaberViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::FriendSaberViewController_bsml, get_transform(), this);
        friendsFirstActivation = true;
    }

    void FriendSaberViewController::PostParse(){
        for (auto scrollView : searchResults->tableView->get_transform()->GetComponentsInChildren<HMUI::ScrollView*>()){
            scrollView->verticalScrollIndicator->get_gameObject()->SetActive(false);
            scrollView->pageUpButton->get_gameObject()->SetActive(false);
            scrollView->pageDownButton->get_gameObject()->SetActive(false);
        }
        for (auto scrollView : friendsList->tableView->get_transform()->GetComponentsInChildren<HMUI::ScrollView*>()){
            scrollView->verticalScrollIndicator->get_gameObject()->SetActive(false);
            scrollView->pageUpButton->get_gameObject()->SetActive(false);
            scrollView->pageDownButton->get_gameObject()->SetActive(false);
        }

        auto resultsTableView = searchResults->tableView;
        auto searchGo = searchResults->get_gameObject();
        Object::DestroyImmediate(searchResults);
        searchResults = nullptr;
        customSearchResults = searchGo->AddComponent<CustomList::FriendSaberListData*>();
        customSearchResults->tableView = resultsTableView;
        resultsTableView->SetDataSource(customSearchResults->i_IDataSource(), false);
        customSearchResults->set_name("SearchList");

        auto friendsTableView = friendsList->tableView;
        auto friendGo = friendsList->get_gameObject();
        Object::DestroyImmediate(friendsList);
        friendsList = nullptr;
        customFriendsList = friendGo->AddComponent<CustomList::FriendSaberListData*>();
        customFriendsList->tableView = friendsTableView;
        friendsTableView->SetDataSource(customFriendsList->i_IDataSource(), false);
        customFriendsList->set_name("FriendsList");

        auto friendsList = Utils::split(readfile(Utils::Constants::FRIENDS_PATH), ',');
        CustomList::FriendSaberListData::globalFriendsList = std::unordered_set<std::string>(friendsList.begin(), friendsList.end());
        CustomList::FriendSaberListData::currentListEdits = std::unordered_set<std::string>(friendsList.begin(), friendsList.end());
        CustomList::FriendSaberListData::localPlayerId = std::stoul(Utils::split(readfile(Utils::Constants::STEAM_KEY_PATH), ':')[1]);

        CustomList::FriendSaberListData::updateSyncability = std::bind(&FriendSaberViewController::UpdateSyncability, this);
        CustomList::FriendSaberListData::deleteFriend = std::bind(&FriendSaberViewController::OpenDeleteModal, this, std::placeholders::_1);

        UpdateSyncability();
    }

    void FriendSaberViewController::OnTabSelect(HMUI::SegmentedControl* control, int index){
        selectedTab = index == 1 ? "friends" : "search";
        if (index == 1){

            OnFriendsTabActivate(friendsFirstActivation, friendsReloadScheduled);
            friendsFirstActivation = false;
            friendsReloadScheduled = false;
        }
    }

    void FriendSaberViewController::OnFriendsTabActivate(bool firstActivation, bool reloadScheduled){
        FriendsListSearch();
    }

    void FriendSaberViewController::FriendsListSearch(){
        bool searchRequired = false;
        bool refreshRequired = false;
        std::vector<Models::FriendSaberUser> currentFriendsInList;
        currentFriendsInList.swap(customFriendsList->userList);
        
        for (int i=0; i<currentFriendsInList.size(); i++){
            auto& player = currentFriendsInList[i];
            if (!CustomList::FriendSaberListData::currentListEdits.contains(std::to_string(player._playerId))){
                refreshRequired = true;
                currentFriendsInList.erase(currentFriendsInList.begin() + i);
                i--;
            }
        }

        std::set<std::string> currentIds;
        std::for_each(currentFriendsInList.begin(), currentFriendsInList.end(), [&](auto& item) { currentIds.insert(std::to_string(item._playerId)); }); 

        std::vector<std::string> idsToSearch;
        for (auto& id : CustomList::FriendSaberListData::currentListEdits){
            if (!currentIds.contains(id)) idsToSearch.push_back(id);
        }

        if (!idsToSearch.empty()) {
            searchRequired = true;
            customFriendsList->tableView->ReloadData();
            SetSearchLoading(friendsLoading, friendError, true);
        }

        customFriendsList->userList.swap(currentFriendsInList);

        if (!searchRequired) {
            if (refreshRequired) customFriendsList->tableView->ReloadDataKeepingPosition();
            SetSearchLoading(friendsLoading, friendError, false, customFriendsList->userList.empty() ? "lmao" : "");
            return;
        }

        completionAmount = 0;
        for (auto& playerId : idsToSearch){
            Downloaders::GetPlayerById(playerId, [this, idsToSearch](std::optional<Models::FriendSaberUser> newFriend){
                completionAmount++;
                if (newFriend.has_value()) customFriendsList->userList.push_back(newFriend.value());
                if (completionAmount == idsToSearch.size()) {
                    std::sort(customFriendsList->userList.begin(), customFriendsList->userList.end(), [this](auto& a, auto& b){
                        if (a._playerId == customFriendsList->localPlayerId) return true;
                        if (b._playerId == customFriendsList->localPlayerId) return false;
                        return Utils::toLower(a._playerName) < Utils::toLower(b._playerName);
                    });
                    customFriendsList->tableView->ReloadData();
                    SetSearchLoading(friendsLoading, friendError, false, customFriendsList->userList.empty() ? "lmao" : "");
                }
            });
        }
    }

    void FriendSaberViewController::OnInput(StringW inputText){
        searchQuery = static_cast<std::string>(inputText);
    }

    void FriendSaberViewController::SetSearchLoading(UnityEngine::GameObject* loader, TMPro::TextMeshProUGUI* text,  bool value, std::string error){
        loader->set_active(value);
        text->get_gameObject()->set_active(!value && error != "");
        if (error == "") return;
        text->SetText("blah");
        text->SetText(error);
    }

    void FriendSaberViewController::OnSearch(){
        customSearchResults->userList.clear();
        customSearchResults->tableView->ReloadData();
        SetSearchLoading(searchLoading, searchError, true);
        Downloaders::GetPlayersByString(searchQuery, [this](std::optional<std::vector<Models::FriendSaberUser>> friends){
            if (!friends.has_value()) return SetSearchLoading(searchLoading, searchError, false, "No Results!");
            SetSearchLoading(searchLoading, searchError, false);
            customSearchResults->userList.swap(friends.value());
            customSearchResults->tableView->ReloadData();
        });
    }

    void FriendSaberViewController::OpenSyncModal(){
        openModalID = "sync";
        confirmButtonsLayout->get_gameObject()->set_active(true);
        closeButtonLayout->get_gameObject()->set_active(false);
        warningText->get_gameObject()->set_active(true);
        warningText->SetText("Would you like to sync your shit with scoresaber");
        syncProgress->set_active(false);
        friendSaberModal->Show(true, true, nullptr);
    }

    void FriendSaberViewController::OpenRevertModal(){
        openModalID = "revert";
        confirmButtonsLayout->get_gameObject()->set_active(true);
        closeButtonLayout->get_gameObject()->set_active(false);
        warningText->get_gameObject()->set_active(true);
        warningText->SetText("Would you like to revert your unsynced changes?");
        syncProgress->set_active(false);
        friendSaberModal->Show(true, true, nullptr);
    }

    void FriendSaberViewController::OpenDeleteModal(Models::FriendSaberUser* player){
        openModalID = string_format("delete:%s:%lu", player->_playerName.c_str(), player->_playerId);
        confirmButtonsLayout->get_gameObject()->set_active(true);
        closeButtonLayout->get_gameObject()->set_active(false);
        warningText->get_gameObject()->set_active(true);
        warningText->SetText(string_format("Would you like to remove %s from your friends list?", player->_playerName.c_str()));
        syncProgress->set_active(false);
        friendSaberModal->Show(true, true, nullptr);
    }

    void FriendSaberViewController::UpdateSyncability(){
        bool hasChanges = CustomList::FriendSaberListData::globalFriendsList != CustomList::FriendSaberListData::currentListEdits;
        syncButton->set_interactable(hasChanges);
        revertButton->set_interactable(hasChanges);
    }

    void FriendSaberViewController::ConfirmModal(){
        if (openModalID == "sync"){
            warningText->get_gameObject()->set_active(false);
            syncProgress->set_active(true);
            confirmButtonsLayout->get_gameObject()->set_active(false);
            CustomList::FriendSaberListData::globalFriendsList = std::unordered_set<std::string>(CustomList::FriendSaberListData::currentListEdits);
            std::string friendData = Utils::setToString(CustomList::FriendSaberListData::globalFriendsList);
            writefile(Utils::Constants::FRIENDS_PATH, friendData);
            using namespace ScoreSaber::Services;
            PlayerService::authState = PlayerService::AuthState::NotStarted;
            WebUtils::cookie = "";
            PlayerService::AuthenticateUser([this](PlayerService::LoginStatus status){
                Lapiz::Utilities::MainThreadScheduler::Schedule([this, status](){
                    warningText->get_gameObject()->set_active(true);
                    syncProgress->set_active(false);
                    if (status == PlayerService::Error) warningText->SetText("Sync failed! consider restarting your game.");
                    else {
                        warningText->SetText("Enjoy your new friends!");
                        auto lb = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::PlatformLeaderboardViewController*>().FirstOrDefault();
                        lb->SetData(lb->difficultyBeatmap);
                    }
                    customFriendsList->tableView->RefreshCells(true, true);
                    customSearchResults->tableView->RefreshCells(true, true);
                    confirmButtonsLayout->get_gameObject()->set_active(false);
                    closeButtonLayout->get_gameObject()->set_active(true);
                    UpdateSyncability();
                });
            });
        }
        else if (openModalID == "revert"){
            CustomList::FriendSaberListData::currentListEdits = std::unordered_set<std::string>(CustomList::FriendSaberListData::globalFriendsList);
            if (selectedTab == "friends") FriendsListSearch();
            customSearchResults->tableView->ReloadDataKeepingPosition();
            warningText->SetText("changes have been reverted");
            confirmButtonsLayout->get_gameObject()->set_active(false);
            closeButtonLayout->get_gameObject()->set_active(true);
            UpdateSyncability();
        }
        else if (openModalID.starts_with("delete")){
            auto splat = Utils::split(openModalID, ':');
            CustomList::FriendSaberListData::currentListEdits.erase(splat[2]);
            FriendsListSearch();
            customSearchResults->tableView->ReloadDataKeepingPosition();
            warningText->SetText(string_format("%s has been deleted =(", splat[1].c_str()));
            confirmButtonsLayout->get_gameObject()->set_active(false);
            closeButtonLayout->get_gameObject()->set_active(true);
            UpdateSyncability();
        }
        else {
            friendSaberModal->Hide(true, nullptr);
        }
    }

    void FriendSaberViewController::Cancel(){
        friendSaberModal->Hide(true, nullptr);
    }
}