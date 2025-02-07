#ifndef OHOS_SKIA_APP_NODECONTENT_H
#define OHOS_SKIA_APP_NODECONTENT_H

#include "tools/sk_app/ohos/XComponentNode.h"

#include <arkui/native_node.h>

namespace sk_app {

class XComponentNode;
class OhosSkiaNode;

class NodeContent {
public:
    static NodeContent* Create(ArkUI_NodeContentHandle contentHandle);
    explicit NodeContent(ArkUI_NodeContentHandle content_handle);
    virtual ~NodeContent();
    
    void DetachRootNode();
    // virtual void setGaneshGL() = 0;
    virtual void setGaneshVk() = 0;
    virtual void setGraphiteVulkan() = 0;
    virtual void setGraphiteDawn() = 0;
    
private:
    virtual void OnRootNodeAttached() = 0;
    virtual void OnRootNodeDetached() = 0;
    void OnAttachToWindow();
    void OnDetachToWindow();
    
    virtual XComponentNode* GetRootNode() = 0;

    const ArkUI_NodeContentHandle fContentHandle;
    bool fVisible = false;
    XComponentNode* fRootNode = nullptr;
};

}  // namespace hello
#endif  // HELLOXCOMPONENT_NODECONTENT_H
