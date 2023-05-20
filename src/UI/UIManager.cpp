#include "UI/UIManager.hpp"
#include "Utils/EasyDelegate.hpp"
#include "main.hpp"
#include "HMUI/ViewController_DidActivateDelegate.hpp"
#include "HMUI/ViewController_DidDeactivateDelegate.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

DEFINE_TYPE(FriendSaber, UIManager);

using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace EasyDelegate;

namespace FriendSaber{
    void UIManager::ctor(PlatformLeaderboardViewController* platformLeaderboardViewController, FriendSaberViewController* friendSaberViewController){
        INVOKE_CTOR();
        _platformLeaderboardViewController = platformLeaderboardViewController;
        _friendSaberViewController = friendSaberViewController;
        getLogger().info("Constructed FriendSaber UIManager");
    }

    void UIManager::Initialize(){
        using ActivateDeleg = HMUI::ViewController::DidActivateDelegate;
        using DeactivateDeleg = HMUI::ViewController::DidDeactivateDelegate;

        _platformLeaderboardViewController->add_didActivateEvent(MakeDelegate<ActivateDeleg*>([this](bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
            if (!firstActivation) return _uiScreen->get_gameObject()->SetActive(true);
            _uiScreen = BSML::FloatingScreen::CreateFloatingScreen({90, 130}, false, {4.5f, 1.5f, -0.6f}, Quaternion::Euler({0, 80, 0}), 0, true);
            _uiScreen->SetRootViewController(_friendSaberViewController, HMUI::ViewController::AnimationType::None);
            _uiScreen->get_gameObject()->set_active(true);
            _uiScreen->rootViewController->__Activate(false, false);
        }));
    
        _platformLeaderboardViewController->add_didDeactivateEvent(MakeDelegate<DeactivateDeleg*>([this](bool removedFromHierarchy, bool screenSystemDisabling){
            _uiScreen->get_gameObject()->set_active(false);
        }));
        getLogger().info("Initialised FriendSaber UIManager");
    }

    void UIManager::Dispose(){
        getLogger().info("Disposed FriendSaber UIManager");
    }
}