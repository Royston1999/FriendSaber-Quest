#pragma once

#include "custom-types/shared/macros.hpp"
#include "System/IDisposable.hpp"
#include "Zenject/IInitializable.hpp"
#include "lapiz/shared/macros.hpp"
#include "UI/FriendSaberViewController.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "bsml/shared/BSML/FloatingScreen/FloatingScreen.hpp"

#define INTERFACES                                                        \
    {                                                                     \
        classof(System::IDisposable*), classof(Zenject::IInitializable*) \
    }

___DECLARE_TYPE_WRAPPER_INHERITANCE(FriendSaber, UIManager, Il2CppTypeEnum::IL2CPP_TYPE_CLASS, Il2CppObject, "FriendSaber", INTERFACES, 0, nullptr,
                                    DECLARE_PRIVATE_FIELD(GlobalNamespace::PlatformLeaderboardViewController*, _platformLeaderboardViewController);
                                    DECLARE_PRIVATE_FIELD(FriendSaber::FriendSaberViewController*, _friendSaberViewController);
                                    DECLARE_PRIVATE_FIELD(BSML::FloatingScreen*, _uiScreen);
                                    DECLARE_CTOR(ctor, 
                                                GlobalNamespace::PlatformLeaderboardViewController* platformLeaderboardViewController, 
                                                FriendSaber::FriendSaberViewController* friendSaberViewController
                                                );
                                    DECLARE_OVERRIDE_METHOD(void, Initialize, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::Zenject::IInitializable::Initialize>::get());
                                    DECLARE_OVERRIDE_METHOD(void, Dispose, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::System::IDisposable::Dispose>::get());
                                    private:

                                    )

#undef INTERFACES