#ifndef NAPI_MANAGER_H_
#define NAPI_MANAGER_H_

#include "tools/sk_app/ohos/XComponentNode.h"

#define NODE_ADDON_API_DISABLE_DEPRECATED
#include "napi.h"

namespace sk_app {

class OhosSkiaApp;

class NapiManager {
public:
    static void Init(const Napi::Env& env);
    static NapiManager* GetInstance();
    static Napi::Value NapiCreateNativeNode(const Napi::CallbackInfo& info);
    static Napi::Value NapiSetSkiaBackend(const Napi::CallbackInfo& info);

    const Napi::Env& env() const { return fEnv; }
    XComponentNode::OhosSkiaApp* ohosSkiaApp() const { return fSkiaApp.get(); }

private:
    NapiManager(const Napi::Env& env);
    ~NapiManager();

    void CreateNativeNode(ArkUI_NodeContentHandle content_handle, bool isGanesh, bool isDawn);
    void SetSkiaBackend(bool isGanesh, bool isDawn);
    void Update();

    Napi::Env fEnv{nullptr};

    std::unique_ptr<NodeContent> fOhosViewer;

    bool fIsGanesh = true;
    bool fIsDawn = true;

    std::unique_ptr<XComponentNode::OhosSkiaApp> fSkiaApp;
};

}  // namespace sk_app

#endif  // NAPI_MANAGER_H_
