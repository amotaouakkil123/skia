#ifndef Window_ohos_DEFINED
#define Window_ohos_DEFINED

#include "tools/sk_app/Window.h"
#include "tools/sk_app/ohos/surface_glue_ohos.h"

namespace sk_app {

class Window_ohos : public Window {
public:
    Window_ohos() : Window() {}
    ~Window_ohos() override {}

    bool init(OhosSkiaApp* ohosSkiaApp);
    void initDisplay(OHNativeWindow* window);
    void onDisplayDestroyed();

    void setRequestedDisplayParams(std::unique_ptr<const skwindow::DisplayParams>,
                                   bool allowReattach) override;

    void setTitle(const char*) override;
    void show() override {}

    bool attach(BackendType) override;
    void onInval() override;
    void setUIState(const char* state) override;

    void paintIfNeeded(); // Do I even need this method?
    bool scaleContentToFit() const override { return true; }

private:
    OhosSkiaApp* fOhosSkiaApp = nullptr;
    BackendType  fBackendType;
};

} // namespace sk_app

#endif