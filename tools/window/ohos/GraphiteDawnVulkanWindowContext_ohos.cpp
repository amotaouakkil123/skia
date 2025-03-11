#include "dawn/native/DawnNative.h"
#include "webgpu/webgpu_cpp.h"

#include "tools/window/GraphiteDawnWindowContext.h"
#include "tools/window/ohos/WindowContextFactory_ohos.h"

#include "tools/sk_app/ohos/logger_common.h"

using skwindow::DisplayParams;
using skwindow::internal::GraphiteDawnWindowContext;

namespace {

class GraphiteDawnVulkanWindowContext_ohos : public GraphiteDawnWindowContext {
public:
    GraphiteDawnVulkanWindowContext_ohos(OHNativeWindow* window, std::unique_ptr<const DisplayParams> params);
    ~GraphiteDawnVulkanWindowContext_ohos() override;

    bool onInitializeContext() override;
    void onDestroyContext() override;
    void resize(int w, int h) override;

private:
    OHNativeWindow* fWindow;
};

GraphiteDawnVulkanWindowContext_ohos::GraphiteDawnVulkanWindowContext_ohos(
    OHNativeWindow* window,
    std::unique_ptr<const DisplayParams> params)
    : GraphiteDawnWindowContext(std::move(params), wgpu::TextureFormat::RGBA8Unorm)
    , fWindow(window) {
    
    if (params == nullptr) {
        LOGD("WindowContext::MakeGraphiteDawnVulkanForOhos Display params are soooooooo screwed");
    } else {
        LOGD("WindowContext::MakeGraphiteDawnVulkanForOhos ooo lala the parameters are great!! :)");
    }
    unsigned int width, height;
    OH_NativeWindow_NativeWindowHandleOpt(window, GET_BUFFER_GEOMETRY, &height, &width);
    this->initializeContext(128, 128);
}

GraphiteDawnVulkanWindowContext_ohos::~GraphiteDawnVulkanWindowContext_ohos() {
    this->destroyContext();
}

bool GraphiteDawnVulkanWindowContext_ohos::onInitializeContext() {
    LOGD("GraphiteDawnVulkanWindowContext_ohos::onInitializeContext entering the context initialization");
    SkASSERT(!!fWindow);

    auto device = this->createDevice(wgpu::BackendType::Vulkan);
    if (!device) {
        SkASSERT(device);
        return false;
    }

    wgpu::SurfaceSourceOHNativeWindow surfaceChainedDesc;
    surfaceChainedDesc.window = fWindow;

    wgpu::SurfaceDescriptor surfaceDesc;
    surfaceDesc.nextInChain = &surfaceChainedDesc;

    auto surface = wgpu::Instance(fInstance->Get()).CreateSurface(&surfaceDesc);
    if (!surface) {
        SkASSERT(false);
        return false;
    }

    fDevice = std::move(device);
    fSurface = std::move(surface);
    configureSurface();

    return true;
}

void GraphiteDawnVulkanWindowContext_ohos::onDestroyContext() {}

void GraphiteDawnVulkanWindowContext_ohos::resize(int w, int h) {
    configureSurface();
}

} // anonymous namespace

namespace skwindow {

std::unique_ptr<WindowContext> MakeGraphiteDawnVulkanForOhos(OHNativeWindow* window,
                                                             std::unique_ptr<const DisplayParams> params) {
    LOGD("Window::OHOS Entering dawn window context creation");
    if (params == nullptr) {
        LOGD("WindowContext::MakeGraphiteDawnVulkanForOhos Display params are ;(((((())))))");
    } else {
        LOGD("WindowContext::MakeGraphiteDawnVulkanForOhos der parameters are splendid yaaa");
    }
    std::unique_ptr<WindowContext> ctx(
            new GraphiteDawnVulkanWindowContext_ohos(window, std::move(params)));
    if (!ctx->isValid()) {
        return nullptr;
    }

    return ctx;
}

}