#pragma once

#include <memory>
#include "abstractGameobject.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

bool imguiVisible = false;

namespace imgui_layer {
  void toggleVisible() {
    imguiVisible = !imguiVisible;
  }
  void init(SDL_Window *window, SDL_Renderer *renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
  }

  void destroy() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  void update() {
  }

  void processEvents(SDL_Event* event) {
    ImGui_ImplSDL2_ProcessEvent(event);
  }

  void drawBegin() {
    if (!imguiVisible) {
      return;
    }

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
  }

  void drawEnd() {
    if (!imguiVisible) {
      return;
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  }

  void debugEntities(std::vector<std::unique_ptr<AbstractGameObject>>* entities) {
    if (!imguiVisible) {
      return;
    }

    ImGui::Begin("Test");
    ImGui::SetWindowFontScale(0.5);

    for(const auto &obj : *entities) {
      ImGui::Text("- Object ID: 0 -");
      ImGui::Text("Tag: %d", obj->getTag());
      ImGui::Text("Type: %d", obj->getType());
      ImGui::Text("position: (%d\t%d)", (int)obj->getPosition().x, (int)obj->getPosition().y);
    }

    ImGui::End();
  }
}
