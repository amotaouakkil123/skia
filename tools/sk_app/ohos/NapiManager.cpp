#include "tools/sk_app/ohos/NapiManager.h"
#include "tools/sk_app/ohos/ohos_log.h"
#include <arkui/native_interface.h>
#include <arkui/native_node_napi.h>

// Skia application manager
namespace sk_app {
namespace {
    NodeContent* skiaNode = nullptr;
    NapiManager* napiManager = nullptr;
}

// Skia application manager methods
void NapiManager::Init(const Napi::Env& env) {
    LOGI("NapiManager::Init");
    CHECK(!napiManager);
    napiManager = new NapiManager(env);
}

NapiManager* NapiManager::GetInstance() {
    LOGI("NapiManager::GetInstance");
    CHECK(napiManager);
    return napiManager;
}

// This is equivalent to OhosSkiaNode.createNative(this.NodeContent)
Napi::Value NapiManager::NapiCreateNativeNode(const Napi::CallbackInfo& info) {
    LOGI("NapiManager::NapiCreateNativeNode");
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
        Napi::Error::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    ArkUI_NodeContentHandle contentHandle = nullptr;
    int32_t ret =
        OH_ArkUI_GetNodeContentFromNapiValue(env, info[0], &contentHandle);
    
    if (ret != ARKUI_ERROR_CODE_NO_ERROR) {
        Napi::Error::New(env, "Arg 0 is not a NodeContent")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    GetInstance()->CreateNativeNode(contentHandle);
    
    return env.Null();
}

// // This is equivalent to OhosSkiaNode.setGanesh()
// Napi::Value NapiManager::NapiSetGaneshGL(const Napi::CallbackInfo& info) {
//     LOGI("NapiManager::NapiSetGaneshGL");
//     Napi::Env env = info.Env();
//     GetInstance()->setGaneshGL();
//     return env.Null();
// }

Napi::Value NapiManager::NapiSetGaneshVk(const Napi::CallbackInfo& info) {
    LOGI("NapiManager::NapiSetGaneshVk");
    Napi::Env env = info.Env();
    GetInstance()->setGaneshVk();
    return env.Null();
}

// This is equivalent to OhosSkiaNode.setGraphiteVulkan()
Napi::Value NapiManager::NapiSetGraphiteVulkan(const Napi::CallbackInfo& info) {
    LOGI("NapiManager::NapiSetGraphiteVulkan");
    Napi::Env env = info.Env();
    GetInstance()->setGraphiteVulkan();
    return env.Null();
}

// This is equivalent to OhosSkiaNode.setGraphiteDawn()
Napi::Value NapiManager::NapiSetGraphiteDawn(const Napi::CallbackInfo& info) {
    LOGI("NapiManager::NapiSetGraphiteDawn");
    Napi::Env env = info.Env();
    GetInstance()->setGraphiteDawn();
    return env.Null();
}

void NapiManager::CreateNativeNode(ArkUI_NodeContentHandle contentHandle) {
    LOGI("NapiManager::CreateNativeNode");
    skiaNode = NodeContent::Create(contentHandle);
}

// void NapiManager::setGaneshGL() {
//     skiaNode->setGaneshGL();
// }

void NapiManager::setGaneshVk() {
    skiaNode->setGaneshVk();
}

void NapiManager::setGraphiteVulkan() {
    skiaNode->setGraphiteVulkan();
}

void NapiManager::setGraphiteDawn() {
    skiaNode->setGraphiteDawn();
}

}