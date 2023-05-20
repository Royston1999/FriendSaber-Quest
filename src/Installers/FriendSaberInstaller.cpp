#include "Installers/FriendSaberInstaller.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "lapiz/shared/utilities/ZenjectExtensions.hpp"
#include "UI/FriendSaberViewController.hpp"
#include "UI/UIManager.hpp"

DEFINE_TYPE(FriendSaber::Installers, FriendSaberInstaller);

using namespace Lapiz::Zenject::ZenjectExtensions;

namespace FriendSaber::Installers
{
    void FriendSaberInstaller::InstallBindings()
    {
        FromNewComponentAsViewController(get_Container()->BindInterfacesAndSelfTo<FriendSaber::FriendSaberViewController*>())->AsSingle();
        get_Container()->BindInterfacesAndSelfTo<FriendSaber::UIManager*>()->AsSingle();
    }
}
