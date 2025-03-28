//
// Created on 2025-03-08.
//
// Node APIs are not fully supported. To solve the compilation error of the
// interface cannot be found, please include "napi/native_api.h".

#ifndef NODECONTENT_H
#define NODECONTENT_H

#include "tools/sk_app/ohos/XComponentNode.h"

#include <arkui/native_node.h>

namespace sk_app {

class NodeContent {
public:
    explicit NodeContent(ArkUI_NodeContentHandle content_handle);
    virtual ~NodeContent();

    virtual void SetVisible(bool visible) = 0;

protected:
    void DetachRootNode();

private:
    virtual XComponentNode* GetRootNode() = 0;
    virtual void OnRootNodeAttached() = 0;
    virtual void OnRootNodeDetached() = 0;

    void OnAttachToWindow();
    void OnDetachToWindow();

    const ArkUI_NodeContentHandle fContentHandle;
    bool fIsVisible = false;
    XComponentNode* fRootNode = nullptr;
};

}  // namespace sk_app
#endif  // NODECONTENT_H
