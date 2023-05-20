#pragma once

#include "HMUI/HoverHint.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/TableCell.hpp"
#include "UI/CustomList/FriendSaberListData.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/EventSystems/BaseEventData.hpp"
#include "UnityEngine/EventSystems/PointerEventData.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"
#include <functional>

#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "Models/FriendSaberUser.hpp"

#ifndef DECLARE_OVERRIDE_METHOD_MATCH
#define DECLARE_OVERRIDE_METHOD_MATCH(retval, method, mptr, ...) \
    DECLARE_OVERRIDE_METHOD(retval, method, il2cpp_utils::il2cpp_type_check::MetadataGetter<mptr>::get(), __VA_ARGS__)
#endif

DECLARE_CLASS_CODEGEN(FriendSaber::CustomList, FriendSaberListCell, HMUI::TableCell,

                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, name);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, sub);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, syncWarning);
                      DECLARE_INSTANCE_FIELD(HMUI::ImageView*, image);
                      DECLARE_INSTANCE_FIELD(BSML::Backgroundable*, backgroundImage);
                      DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, toggle_friend);

                      DECLARE_INSTANCE_FIELD(FriendSaberListData*, tableData);

                      DECLARE_INSTANCE_METHOD(void, PostParse);
                      DECLARE_INSTANCE_METHOD(void, ToggleFriend);

                      DECLARE_INSTANCE_METHOD(void, FixedUpdate);
                      DECLARE_OVERRIDE_METHOD_MATCH(void, HighlightDidChange, &HMUI::SelectableCell::HighlightDidChange, HMUI::SelectableCell::TransitionType transitionType);

                      public:

                      static FriendSaber::CustomList::FriendSaberListCell* CreateNewCell();

                      void set_name(std::string_view name);
                      void set_sub(std::string_view sub);
                      void set_sync_active();
                      void set_sprite(UnityEngine::Sprite* preview);
                      void set_button_text(std::string text);
                      void Populate(FriendSaber::Models::FriendSaberUser& user);
                      bool downloadInProgress;                        

                      FriendSaber::Models::FriendSaberUser* _user;
)