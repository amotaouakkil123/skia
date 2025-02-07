#include "tools/sk_app/ohos/OhosSkiaNode.h"
#include "tools/sk_app/ohos/NapiManager.h"
#include "tools/sk_app/ohos/Window_ohos.h"
#include "tools/sk_app/ohos/ohos_log.h"

#include <arkui/native_node.h>
#include <arkui/native_node_napi.h>
#include <arkui/native_interface.h>
#include <arkui/ui_input_event.h>


namespace sk_app {

NodeContent* NodeContent::Create(ArkUI_NodeContentHandle contentHandle) {
    return new OhosSkiaNode(contentHandle);
}

OhosSkiaNode::OhosSkiaNode(ArkUI_NodeContentHandle contentHandle) 
    : NodeContent(contentHandle) {
    LOGI("OhosSkiaNode::OhosSkiaNode");
    fRootNode = XComponentNode::Create("OhosSkiaApp");
    fRootNode->SetWidthPercent(1);
    fRootNode->SetHeightPercent(1);
}

OhosSkiaNode::~OhosSkiaNode() {
    LOGI("OhosSkiaNode::~OhosSkiaNode");
}

XComponentNode* OhosSkiaNode::GetRootNode() {
    LOGI("OhosSkiaNode::GetRootNode");
    return fRootNode.get();
}

void OhosSkiaNode::OnRootNodeAttached() {
    LOGI("OhosSkiaNode::OnRootNodeAttached");
}

void OhosSkiaNode::OnRootNodeDetached() {
    LOGI("OhosSkiaNode::OnRootNodeDetached");
}

// void OhosSkiaNode::setGaneshGL() {
//     fRootNode->setGaneshGL();
// }

void OhosSkiaNode::setGaneshVk() {
    fRootNode->setGaneshVk();
}

void OhosSkiaNode::setGraphiteVulkan() {
    fRootNode->setGraphiteVulkan();
}

void OhosSkiaNode::setGraphiteDawn() {
    fRootNode->setGraphiteDawn();
}

} // namespace sk_app
