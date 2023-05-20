#include "UI/CustomList/FriendSaberListData.hpp"
#include "UI/CustomList/FriendSaberListCell.hpp"
#include "assets.hpp"

#include "System/Action_2.hpp"
#include "bsml/shared/Helpers/utilities.hpp"

#include "custom-types/shared/delegate.hpp"

#include "Utils/Constants.hpp"
#include "Utils/StringUtils.hpp"

DEFINE_TYPE(FriendSaber::CustomList, FriendSaberListData);

namespace FriendSaber::CustomList
{
    std::unordered_set<std::string> FriendSaberListData::globalFriendsList;
    std::unordered_set<std::string> FriendSaberListData::currentListEdits;
    unsigned long FriendSaberListData::localPlayerId;
    std::function<void()> FriendSaberListData::updateSyncability;
    std::function<void(Models::FriendSaberUser*)> FriendSaberListData::deleteFriend;

    void FriendSaberListData::ctor()
    {
        INVOKE_CTOR();
        reuseIdentifier = StringW("FriendSaberCellList");
        cellSize = 12.0f;

    }

    void FriendSaberListData::Start()
    {
        
    }

    float FriendSaberListData::CellSize()
    {
        return cellSize;
    }

    int FriendSaberListData::NumberOfCells()
    {
        return userList.size();
    }

    HMUI::TableCell* FriendSaberListData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        auto tableCell = reinterpret_cast<FriendSaberListCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));

        if (!tableCell)
        {
            tableCell = FriendSaberListCell::CreateNewCell();
        }

        tableCell->tableData = this;
        tableCell->Populate(userList[idx]);
        // tableCell->deletionConfirmationModal = deletionConfirmationModal;
        tableCell->set_reuseIdentifier(reuseIdentifier);

        return tableCell;
    }

    HMUI::TableView::IDataSource* FriendSaberListData::i_IDataSource() { return reinterpret_cast<HMUI::TableView::IDataSource*>(this); }

}