#ifndef OHOS_SKIA_NODE_DEFINED
#define OHOS_SKIA_NODE_DEFINED

#include <napi/native_api.h>
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <arkui/native_node.h>
#include <native_window/external_window.h>
#include <napi/native_api.h>

#include <string>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <memory>

#include "include/core/SkString.h"
#include "tools/sk_app/ohos/NodeContent.h"
#include "tools/sk_app/Application.h"
#include "tools/sk_app/Window.h"
#include "tools/sk_app/ohos/XComponentNode.h"

namespace sk_app {

class XComponentNode;
class NodeContent;

class OhosSkiaNode : public NodeContent {
public:
    OhosSkiaNode(ArkUI_NodeContentHandle contentHandle);
    virtual ~OhosSkiaNode();

    // Methods to set Skia backend
    // void setGaneshGL() override;
    void setGaneshVk() override;
    void setGraphiteVulkan() override;
    void setGraphiteDawn() override;
    
    
private:
    XComponentNode* GetRootNode() override;
    void OnRootNodeAttached() override;
    void OnRootNodeDetached() override;

    std::unique_ptr<XComponentNode> fRootNode;

};

} // namespace sk_app

#endif
