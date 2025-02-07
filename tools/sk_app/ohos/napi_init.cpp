#define NODE_ADDON_API_DISABLE_DEPRECATED

#ifndef NAPI_DISABLE_CPP_EXCEPTIONS
#define NAPI_DISABLE_CPP_EXCEPTIONS
#endif

#include "tools/sk_app/ohos/napi.h"
#include "tools/sk_app/ohos/NapiManager.h"
#include "tools/sk_app/ohos/ohos_log.h"

namespace viewer {

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    LOGI("napi_init::Init");
    sk_app::NapiManager::Init(env);
    exports.Set(
        "createNativeNode",
        Napi::Function::New<sk_app::NapiManager::NapiCreateNativeNode>(env));
    // exports.Set(
    //     "setGaneshGL",
    //     Napi::Function::New<sk_app::NapiManager::NapiSetGaneshGL>(env));
    exports.Set(
        "setGaneshVk",
        Napi::Function::New<sk_app::NapiManager::NapiSetGaneshVk>(env));
    exports.Set(
        "setGraphiteVulkan",
        Napi::Function::New<sk_app::NapiManager::NapiSetGraphiteVulkan>(env));
    exports.Set(
        "setGraphiteDawn",
        Napi::Function::New<sk_app::NapiManager::NapiSetGraphiteDawn>(env));

    return exports;
}

}  // namespace helloxcomponent

using viewer::Init;

NODE_API_MODULE(viewer, Init)
