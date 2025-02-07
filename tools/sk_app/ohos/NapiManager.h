#ifndef SKIA_APP_MANAGER_DEFINED
#define SKIA_APP_MANAGER_DEFINED

#ifndef NAPI_DISABLE_CPP_EXCEPTIONS
#define NAPI_DISABLE_CPP_EXCEPTIONS
#endif

#define NODE_ADDON_API_DISABLE_DEPRECATED

#include <napi/native_api.h>
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <arkui/native_node.h>
#include <native_window/external_window.h>

#include <string>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <memory>

#include "tools/sk_app/ohos/napi.h"
#include "tools/sk_app/ohos/NodeContent.h"

namespace sk_app {

class OhosSkiaNode;

class NapiManager {
public:
    NapiManager(const Napi::Env& env) {}
    ~NapiManager();
    static void Init(const Napi::Env& env);
    static NapiManager* GetInstance();
    static Napi::Value NapiCreateNativeNode(const Napi::CallbackInfo& info);
    // The napi ganesh, graphite, and dawn methods are generic in their intent
    // They do not impose any restrictions on what kind of application we seek to build
    // static Napi::Value NapiSetGaneshGL(const Napi::CallbackInfo& info);
    static Napi::Value NapiSetGaneshVk(const Napi::CallbackInfo& info);
    static Napi::Value NapiSetGraphiteVulkan(const Napi::CallbackInfo& info);
    static Napi::Value NapiSetGraphiteDawn(const Napi::CallbackInfo& info);

    void CreateNativeNode(ArkUI_NodeContentHandle contentHandle);

private:
    // void setGaneshGL();
    void setGaneshVk();
    void setGraphiteVulkan();
    void setGraphiteDawn();

};

} // namespace sk_app

#endif
