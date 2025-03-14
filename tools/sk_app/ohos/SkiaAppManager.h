#ifndef SKIA_APP_MANAGER_DEFINED
#define SKIA_APP_MANAGER_DEFINED

#include <napi/native_api.h>
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <arkui/native_node.h>
#include <native_window/external_window.h>

#include <string>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <memory>

#include "tools/sk_app/ohos/surface_glue_ohos.h"

namespace sk_app {

class OhosSkiaApp;

class SkiaAppManager {

public:

    ~SkiaAppManager() = default;

    static SkiaAppManager* GetInstance() {
        static SkiaAppManager instance;
        return &instance;
    }
    
    bool Init(napi_env env, napi_value exports);
    void SetNativeXComponent(std::string& id, OH_NativeXComponent* nativeXComponent);
    OhosSkiaApp* GetRender(std::string& id, ArkUI_NodeHandle handle, ArkUI_NativeNodeAPI_1* nodeApi);

private:

    SkiaAppManager() = default;
    std::unordered_map<std::string, OH_NativeXComponent*> nativeXComponentMap;
    std::unordered_map<std::string, OhosSkiaApp*> ohosSkiaMap;

};

} // namespace sk_app

#endif
