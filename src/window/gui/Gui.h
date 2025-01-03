#pragma once

#ifdef __cplusplus
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <imgui.h>
#include <imgui_internal.h>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>
#include "window/gui/ConsoleWindow.h"
#include "window/gui/InputEditorWindow.h"
#include "window/gui/IconsFontAwesome4.h"
#include "window/gui/GameOverlay.h"
#include "window/gui/StatsWindow.h"
#include "window/gui/GuiWindow.h"
#include "window/gui/GuiMenuBar.h"
#include "libultraship/libultra/controller.h"
#include "resource/type/Texture.h"
#include "window/gui/resource/GuiTexture.h"

namespace Ship {

typedef struct {
    union {
        struct {
            void* Window;
            void* DeviceContext;
            void* Device;
        } Dx11;
        struct {
            void* Window;
            void* Context;
        } Opengl;
        struct {
            void* Window;
            SDL_Renderer* Renderer;
        } Metal;
        struct {
            uint32_t Width;
            uint32_t Height;
        } Gx2;
    };
} GuiWindowInitData;

typedef union {
    struct {
        void* Handle;
        int Msg;
        int Param1;
        int Param2;
    } Win32;
    struct {
        void* Event;
    } Sdl;
    struct {
        void* Input;
    } Gx2;
} WindowEvent;

class Gui {
  public:
    Gui();
    Gui(std::vector<std::shared_ptr<GuiWindow>> guiWindows);
    ~Gui();

    void Init(GuiWindowInitData windowImpl);
    void Draw();
    void HandleWindowEvents(WindowEvent event);
    void SetupRendererFrame();
    void SaveConsoleVariablesNextFrame();
    bool SupportsViewports();

    void AddGuiWindow(std::shared_ptr<GuiWindow> guiWindow);
    std::shared_ptr<GuiWindow> GetGuiWindow(const std::string& name);
    void RemoveGuiWindow(std::shared_ptr<GuiWindow> guiWindow);
    void RemoveGuiWindow(const std::string& name);
    void RemoveAllGuiWindows();

    void LoadGuiTexture(const std::string& name, const std::string& path, const ImVec4& tint);
    bool HasTextureByName(const std::string& name);
    void LoadGuiTexture(const std::string& name, const Fast::Texture& tex, const ImVec4& tint);
    void UnloadTexture(const std::string& name);
    ImTextureID GetTextureByName(const std::string& name);
    ImVec2 GetTextureSize(const std::string& name);
    void LoadTextureFromRawImage(const std::string& name, const std::string& path);

    std::shared_ptr<GameOverlay> GetGameOverlay();
    void SetMenuBar(std::shared_ptr<GuiMenuBar> menuBar);
    std::shared_ptr<GuiMenuBar> GetMenuBar();
    void SetMenu(std::shared_ptr<GuiWindow> menu);
    std::shared_ptr<GuiWindow> GetMenu();
    bool GetMenuOrMenubarVisible();

    bool GamepadNavigationEnabled();
    void BlockGamepadNavigation();
    void UnblockGamepadNavigation();

  protected:
    void StartFrame();
    void EndFrame();
    void DrawFloatingWindows();
    void DrawMenu();
    void DrawGame();

    void ImGuiBackendNewFrame();
    void ImGuiWMNewFrame();
    void ImGuiWMInit();
    void ImGuiBackendInit();
    void ImGuiRenderDrawData(ImDrawData* data);

    ImTextureID GetTextureById(int32_t id);
    void ApplyResolutionChanges();
    int16_t GetIntegerScaleFactor();
    void CheckSaveCvars();

  private:
    GuiWindowInitData mImpl;
    ImGuiIO* mImGuiIo;
    bool mNeedsConsoleVariableSave;
    std::shared_ptr<GameOverlay> mGameOverlay;
    std::shared_ptr<GuiMenuBar> mMenuBar;
    std::shared_ptr<GuiWindow> mMenu;
    std::unordered_map<std::string, GuiTextureMetadata> mGuiTextures;
    std::map<std::string, std::shared_ptr<GuiWindow>> mGuiWindows;
    ImVec2 mTemporaryWindowPos;
};
} // namespace Ship

#endif
