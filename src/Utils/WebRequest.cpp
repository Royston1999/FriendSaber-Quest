#include "Utils/WebRequest.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "main.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Vector4.hpp"
#include "Utils/StringUtils.hpp"

namespace FriendSaber::Utils{
    using namespace UnityEngine;
    using namespace UnityEngine::Networking;
    void SendWebRequest(std::string URL, std::function<void(std::string)> callback){
        Lapiz::Utilities::MainThreadScheduler::Schedule([=](){
            auto request = UnityEngine::Networking::UnityWebRequest::Get(URL);
            request->SetRequestHeader("User-Agent", std::string(MOD_ID) + " " + VERSION);
            request->SendWebRequest()->add_completed(DLCompletedDeleg([=](auto* value){
                callback(request->get_downloadHandler()->GetText());
            }));
        });
    }

    void SendImageRequest(std::string URL, std::function<void(Sprite*)> callback){
        Lapiz::Utilities::MainThreadScheduler::Schedule([=](){
            auto request = UnityWebRequestTexture::GetTexture(URL);
            request->SetRequestHeader("User-Agent", std::string(MOD_ID) + " " + VERSION);
            request->SendWebRequest()->add_completed(DLCompletedDeleg([=](auto* value){
                if (request->get_isHttpError() || request->get_isNetworkError()) return callback(nullptr);
                auto downloadHandlerTexture = reinterpret_cast<DownloadHandlerTexture*>(request->get_downloadHandler());
                auto texture = downloadHandlerTexture->get_texture();
                if (texture == nullptr) return callback(nullptr);
                auto sprite = Sprite::Create(texture, 
                                            Rect(0.0f, 0.0f, 
                                            (float)texture->get_width(), 
                                            (float)texture->get_height()), 
                                            Vector2(0.5f, 0.5f), 
                                            1024.0f, 
                                            1u, 
                                            SpriteMeshType::FullRect, 
                                            Vector4(0.0f, 0.0f, 0.0f, 0.0f), 
                                            false);
                callback(sprite);
            }));
        });
    }
}