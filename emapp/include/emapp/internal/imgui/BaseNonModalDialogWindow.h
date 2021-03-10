/*
   Copyright (c) 2015-2021 hkrn All rights reserved

   This file is part of emapp component and it's licensed under Mozilla Public License. see LICENSE.md for more details.
 */

#pragma once
#ifndef NANOEM_EMAPP_INTERNAL_IMGUI_BASENONMODALDIALOGWINDOW_H_
#define NANOEM_EMAPP_INTERNAL_IMGUI_BASENONMODALDIALOGWINDOW_H_

#include "emapp/internal/ImGuiWindow.h"

namespace nanoem {
namespace internal {
namespace imgui {

struct BaseNonModalDialogWindow : ImGuiWindow::INoModalDialogWindow {
    enum ResponseType {
        kResponseTypeIndetermine,
        kResponseTypeOK,
        kResponseTypeCancel,
    };
    static ImVec2 calcExpandedImageSize(int width, int height) NANOEM_DECL_NOEXCEPT;
    static ImVec2 calcExpandedImageSize(const sg_image_desc &desc) NANOEM_DECL_NOEXCEPT;
    static void detectUpDown(bool &up, bool &down) NANOEM_DECL_NOEXCEPT;
    static void selectIndex(
        bool up, bool down, const nanoem_rsize_t numObjects, nanoem_rsize_t &offset) NANOEM_DECL_NOEXCEPT;
    static void selectIndex(bool up, bool down, const nanoem_rsize_t numObjects, int &offset) NANOEM_DECL_NOEXCEPT;
    static void addSeparator();

    BaseNonModalDialogWindow(BaseApplicationService *applicationPtr);
    ~BaseNonModalDialogWindow();

    bool open(const char *title, const char *id, bool *visible, const ImVec2 size, ImGuiWindowFlags flags);
    bool open(const char *title, const char *id, bool *visible, nanoem_f32_t height = 0, ImGuiWindowFlags flags = 0);
    void close();
    ResponseType layoutCommonButtons(bool *visible);
    const char *tr(const char *text) const NANOEM_DECL_NOEXCEPT;

    BaseApplicationService *m_applicationPtr;
};

} /* namespace imgui */
} /* namespace internal */
} /* namespace nanoem */

#endif /* NANOEM_EMAPP_INTERNAL_IMGUI_BASENONMODALDIALOGWINDOW_H_ */