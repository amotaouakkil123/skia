#include "tools/sk_app/ohos/NodeContent.h"
#include "tools/sk_app/ohos/ohos_log.h"

namespace sk_app {

NodeContent::NodeContent(ArkUI_NodeContentHandle content_handle)
    : fContentHandle(content_handle) {
    FATAL_IF(!XComponentNode::api(), "XComponentNode::api() failed");

    int32_t retval = OH_ArkUI_NodeContent_SetUserData(content_handle, this);
    FATAL_IF(retval != ARKUI_ERROR_CODE_NO_ERROR,
           "OH_ArkUI_NodeContent_SetUserData() failed");

    auto node_content_callback = [](ArkUI_NodeContentEvent* event) {
        ArkUI_NodeContentHandle content_handle =
            OH_ArkUI_NodeContentEvent_GetNodeContentHandle(event);
        auto* self = reinterpret_cast<NodeContent*>(
            OH_ArkUI_NodeContent_GetUserData(content_handle));
        switch (OH_ArkUI_NodeContentEvent_GetEventType(event)) {
            case NODE_CONTENT_EVENT_ON_ATTACH_TO_WINDOW:
                self->OnAttachToWindow();
                break;
            case NODE_CONTENT_EVENT_ON_DETACH_FROM_WINDOW:
                self->OnDetachToWindow();
                break;
        }
    };

    retval = OH_ArkUI_NodeContent_RegisterCallback(content_handle,
                                                   node_content_callback);
    FATAL_IF(retval != ARKUI_ERROR_CODE_NO_ERROR,
           "OH_ArkUI_NodeContent_RegisterCallback() failed");
}

NodeContent::~NodeContent() {
    CHECK(!fRootNode);
}

void NodeContent::DetachRootNode() {
    if (fRootNode) {
        OH_ArkUI_NodeContent_RemoveNode(fContentHandle, fRootNode->handle());
        fRootNode = nullptr;
        OnRootNodeDetached();
    }
}

void NodeContent::OnAttachToWindow() {
    if (!fRootNode) {
        fRootNode = GetRootNode();
        OH_ArkUI_NodeContent_AddNode(fContentHandle, fRootNode->handle());
        OnRootNodeAttached();
    }
}

void NodeContent::OnDetachToWindow() {
    DetachRootNode();
}

}  // namespace hello
