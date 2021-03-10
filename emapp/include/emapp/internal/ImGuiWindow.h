/*
   Copyright (c) 2015-2021 hkrn All rights reserved

   This file is part of emapp component and it's licensed under Mozilla Public License. see LICENSE.md for more details.
 */

#pragma once
#ifndef NANOEM_EMAPP_INTERNAL_IMGUIWINDOW_H_
#define NANOEM_EMAPP_INTERNAL_IMGUIWINDOW_H_

#include "emapp/internal/IUIWindow.h"

#include "emapp/IPrimitive2D.h"
#include "emapp/ITrack.h"

#include "imgui/imgui.h"

namespace nanoem {

class IVectorValueState;

namespace internal {

class AccessoryOpacityValueState;
class AccessoryOrientationVectorValueState;
class AccessoryScaleFactorValueState;
class AccessoryTranslationVectorValueState;
class BoneOrientationValueState;
class BoneTranslationValueState;
class CameraAngleVectorValueState;
class CameraDistanceVectorValueState;
class CameraFovVectorValueState;
class CameraLookAtVectorValueState;
class DraggingMorphSliderState;
class ImGuiApplicationMenuBuilder;
class LightColorVectorValueState;
class LightDirectionVectorValueState;

class ImGuiWindow NANOEM_DECL_SEALED : public IUIWindow, private NonCopyable {
public:
    static const Vector2UI16 kMinimumWindowSize;
    static const nanoem_f32_t kFontSize;
    static const nanoem_f32_t kLeftPaneWidth;
    static const nanoem_f32_t kTranslationStepFactor;
    static const nanoem_f32_t kOrientationStepFactor;
    static const nanoem_f32_t kTimelineDefaultWidthRatio;
    static const nanoem_f32_t kTimelineMinWidthRatio;
    static const nanoem_f32_t kTimelineMaxWidthRatio;
    static const nanoem_f32_t kTimelineSnapGridRatio;
    static const ImGuiDataType kFrameIndexDataType;
    static const ImU32 kMainWindowFlags;
    static const ImU32 kColorWindowBg;
    static const ImU32 kColorMenuBarBg;
    static const ImU32 kColorTitleBg;
    static const ImU32 kColorTitleBgActive;
    static const ImU32 kColorTab;
    static const ImU32 kColorTabActive;
    static const ImU32 kColorTabHovered;
    static const ImU32 kColorResizeGrip;
    static const ImU32 kColorResizeGripHovered;
    static const ImU32 kColorResizeGripActive;
    static const ImU32 kColorChildBg;
    static const ImU32 kColorPopupBg;
    static const ImU32 kColorBorder;
    static const ImU32 kColorText;
    static const ImU32 kColorTextDisabled;
    static const ImU32 kColorButton;
    static const ImU32 kColorButtonActive;
    static const ImU32 kColorButtonHovered;
    static const ImU32 kColorFrameBg;
    static const ImU32 kColorFrameBgActive;
    static const ImU32 kColorFrameBgHovered;
    static const ImU32 kColorHeader;
    static const ImU32 kColorHeaderActive;
    static const ImU32 kColorHeaderHovered;
    static const ImU32 kColorCheckMark;
    static const ImU32 kColorSliderGrab;
    static const ImU32 kColorSliderGrabActive;
    static const ImU32 kColorScrollbarBg;
    static const ImU32 kColorScrollbarGrab;
    static const ImU32 kColorScrollbarGrabActive;
    static const ImU32 kColorScrollbarGrabHovered;
    static const ImU32 kColorRhombusSelect;
    static const ImU32 kColorRhombusDefault;
    static const ImU32 kColorRhombusMoving;
    static const ImU32 kColorInterpolationCurveBezierLine;
    static const ImU32 kColorInterpolationCurveControlLine;
    static const ImU32 kColorInterpolationCurveControlPoint;
    static const nanoem_u8_t kFAArrows[];
    static const nanoem_u8_t kFARefresh[];
    static const nanoem_u8_t kFAZoom[];
    static const nanoem_u8_t kFAMagicWand[];
    static const nanoem_u8_t kFATHList[];
    static const nanoem_u8_t kFAPencil[];
    static const nanoem_u8_t kFAForward[];
    static const nanoem_u8_t kFABackward[];
    static const nanoem_u8_t kFACogs[];
    static const nanoem_u8_t kFAPlus[];
    static const nanoem_u8_t kFAMinus[];
    static const nanoem_u8_t kFAArrowUp[];
    static const nanoem_u8_t kFAArrowDown[];
    static const nanoem_u8_t kFAFolderOpen[];
    static const nanoem_u8_t kFAFolderClose[];
    static const nanoem_u8_t kFACircle[];

    struct ILazyExecutionCommand {
        virtual ~ILazyExecutionCommand() NANOEM_DECL_NOEXCEPT
        {
        }
        virtual void execute(Project *project) = 0;
    };
    typedef tinystl::vector<ILazyExecutionCommand *, TinySTLAllocator> LazyExecutionCommandList;
    struct INoModalDialogWindow {
        virtual ~INoModalDialogWindow() NANOEM_DECL_NOEXCEPT
        {
        }
        virtual bool draw(Project *project) = 0;
    };
    typedef tinystl::unordered_map<const char *, INoModalDialogWindow *, TinySTLAllocator> NoModalDialogWindowList;

    static bool handleButton(const char *label);
    static bool handleButton(const char *label, nanoem_f32_t width, bool enabled);
    static bool handleArrowButton(const char *label, ImGuiDir dir, bool enabled);
    static bool handleRadioButton(const char *label, bool value, bool enabled);
    static bool handleCheckBox(const char *label, bool *value, bool enabled);
    static void saveDefaultStyle(nanoem_f32_t devicePixelRatio);
    static void restoreDefaultStyle();

    ImGuiWindow(BaseApplicationService *application);
    ~ImGuiWindow();

    bool handleTranslatedButton(const char *id);
    bool handleTranslatedButton(const char *id, nanoem_f32_t width, bool enabled);
    void resetSelectionIndex();
    void cancelAllDraggingStates();
    void reloadActiveEffect(IDrawable *value);
    void addLazyExecutionCommand(ILazyExecutionCommand *command);
    void drawTextTooltip(const char *text);
    void openBoneKeyframeBezierInterpolationCurveGraphDialog(Project *project);
    void openCameraKeyframeBezierInterpolationCurveGraphDialog(Project *project);
    void openModelOutsideParentDialog(Project *project);
    void openAccessoryOutsideParentDialog(Project *project);
    void openSelfShadowConfigurationDialog(Project *project);

    void initialize(nanoem_f32_t devicePixelRatio) NANOEM_DECL_OVERRIDE;
    void reset() NANOEM_DECL_OVERRIDE;
    void destroy() NANOEM_DECL_NOEXCEPT_OVERRIDE;
    void setFontPointSize(nanoem_f32_t pointSize) NANOEM_DECL_OVERRIDE;
    void setKeyPressed(BaseApplicationService::KeyType key) NANOEM_DECL_OVERRIDE;
    void setKeyReleased(BaseApplicationService::KeyType key) NANOEM_DECL_OVERRIDE;
    void setUnicodePressed(nanoem_u32_t key) NANOEM_DECL_OVERRIDE;
    void setCurrentCPUPercentage(nanoem_f32_t value) NANOEM_DECL_OVERRIDE;
    void setCurrentMemoryBytes(nanoem_u64_t value) NANOEM_DECL_OVERRIDE;
    void setMaxMemoryBytes(nanoem_u64_t value) NANOEM_DECL_OVERRIDE;
    void setScreenCursorPress(int type, int x, int y, int modifiers) NANOEM_DECL_OVERRIDE;
    void setScreenCursorMove(int x, int y, int modifiers) NANOEM_DECL_OVERRIDE;
    void setScreenCursorRelease(int type, int x, int y, int modifiers) NANOEM_DECL_OVERRIDE;

    void openBoneParametersDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openBoneCorrectionDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openBoneBiasDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openCameraParametersDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openCameraCorrectionDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openMorphCorrectionDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openViewportDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openModelDrawOrderDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openModelTransformOrderDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openPhysicsEngineDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openModelAllObjectsDialog() NANOEM_DECL_OVERRIDE;
    void openModelEdgeDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openScaleAllSelectedKeyframesDialog() NANOEM_DECL_OVERRIDE;
    void openEffectParameterDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openModelParameterDialog(Project *project) NANOEM_DECL_OVERRIDE;
    void openPreferenceDialog() NANOEM_DECL_OVERRIDE;
    bool openModelIOPluginDialog(Project *project, plugin::ModelIOPlugin *plugin, const ByteArray &input,
        const ByteArray &layout, int functionIndex) NANOEM_DECL_OVERRIDE;
    bool openMotionIOPluginDialog(Project *project, plugin::MotionIOPlugin *plugin, const ByteArray &input,
        const ByteArray &layout, int functionIndex) NANOEM_DECL_OVERRIDE;

    void resizeDevicePixelWindowSize(const Vector2UI16 &value) NANOEM_DECL_OVERRIDE;
    void setDevicePixelRatio(float value) NANOEM_DECL_OVERRIDE;
    void drawAll2DPrimitives(
        Project *project, Project::IViewportOverlay *overlay, nanoem_u32_t flags) NANOEM_DECL_OVERRIDE;
    void drawAllWindows(Project *project, nanoem_u32_t flags) NANOEM_DECL_OVERRIDE;
    void drawWindow(Project *project, ImDrawData *drawData, bool load);

    const IModalDialog *currentModalDialog() const NANOEM_DECL_NOEXCEPT_OVERRIDE;
    IModalDialog *currentModalDialog() NANOEM_DECL_NOEXCEPT_OVERRIDE;
    void addModalDialog(IModalDialog *value) NANOEM_DECL_OVERRIDE;
    void clearAllModalDialogs() NANOEM_DECL_OVERRIDE;
    bool hasModalDialog() const NANOEM_DECL_NOEXCEPT_OVERRIDE;
    void swapModalDialog(IModalDialog *value) NANOEM_DECL_OVERRIDE;
    bool isVisible() const NANOEM_DECL_NOEXCEPT_OVERRIDE;
    void setVisible(bool value) NANOEM_DECL_OVERRIDE;
    IPrimitive2D *primitiveContext() NANOEM_DECL_NOEXCEPT_OVERRIDE;

    bool isSGXDebuggerEnabled() const NANOEM_DECL_NOEXCEPT;
    void setSGXDebbugerEnabled(bool value);

private:
    typedef tinystl::vector<IModalDialog *, TinySTLAllocator> ModalDialogList;
    typedef tinystl::unordered_set<ITrack *, TinySTLAllocator> TrackSet;
    BX_ALIGN_DECL_16(struct)
    VertexUnit
    {
        Vector4 m_position;
        Vector3 m_uv;
        glm::u8vec4 m_color;
    };
    struct Buffer {
        Buffer();
        ~Buffer() NANOEM_DECL_NOEXCEPT;
        void update(const ImDrawData *drawData, sg_bindings &bindingsRef);
        void map(size_t estimatedNumVertices, size_t estimatedNumIndices);
        void unmap();
        void destroy() NANOEM_DECL_NOEXCEPT;
        VertexUnit *m_vertexDataPtr;
        ImDrawIdx *m_indexDataPtr;
        sg_buffer m_vertexHandle;
        sg_buffer m_indexHandle;
        tinystl::vector<VertexUnit, TinySTLAllocator> m_vertexData;
        tinystl::vector<ImDrawIdx, TinySTLAllocator> m_indexData;
        bool m_needsUnmapBuffer;
    };
    typedef tinystl::unordered_map<ImGuiID, Buffer, TinySTLAllocator> BufferMap;
    class PrimitiveContext : public IPrimitive2D {
    public:
        PrimitiveContext();
        ~PrimitiveContext() NANOEM_DECL_NOEXCEPT;

        void setBaseOffset(const ImVec2 &value);
        void setScaleFactor(nanoem_f32_t value);

        void strokeLine(
            const Vector2 &from, const Vector2 &to, const Vector4 &color, nanoem_f32_t thickness) NANOEM_DECL_OVERRIDE;
        void strokeRect(const Vector4 &rect, const Vector4 &color, nanoem_f32_t roundness,
            nanoem_f32_t thickness) NANOEM_DECL_OVERRIDE;
        void fillRect(const Vector4 &rect, const Vector4 &color, nanoem_f32_t roundness) NANOEM_DECL_OVERRIDE;
        void strokeCircle(const Vector4 &rect, const Vector4 &color, nanoem_f32_t thickness) NANOEM_DECL_OVERRIDE;
        void fillCircle(const Vector4 &rect, const Vector4 &color) NANOEM_DECL_OVERRIDE;
        void strokeCurve(const Vector2 &a, const Vector2 &c0, const Vector2 &c1, const Vector2 &b, const Vector4 &color,
            nanoem_f32_t thickness) NANOEM_DECL_OVERRIDE;
        void drawTooltip(const char *text, size_t length) NANOEM_DECL_OVERRIDE;

    private:
        ImVec2 m_offset;
        nanoem_f32_t m_scaleFactor;
    };
    struct ScreenCursor {
        ScreenCursor()
            : m_modifiers(Project::kCursorModifierTypeMaxEnum)
            , m_x(0)
            , m_y(0)
        {
            memset(m_pressed, 0, sizeof(m_pressed));
        }
        inline void
        assign(int x, int y, int modifiers) NANOEM_DECL_NOEXCEPT
        {
            m_x = x;
            m_y = y;
            m_modifiers = static_cast<Project::CursorModifierType>(modifiers);
        }
        inline void
        assign(int x, int y, int modifiers, int type, bool pressed) NANOEM_DECL_NOEXCEPT
        {
            assign(x, y, modifiers);
            m_pressed[glm::clamp(type, int(Project::kCursorTypeFirstEnum), int(Project::kCursorTypeMaxEnum) - 1)] =
                pressed;
        }
        Project::CursorModifierType m_modifiers;
        bool m_pressed[Project::kCursorTypeMaxEnum];
        int m_x;
        int m_y;
    };

    enum RhombusReactionType {
        kRhombusReactionNone,
        kRhombusReactionClicked,
        kRhombusReactionDragged,
        kRhombusReactionMoving,
        kRhombusReactionMaxEnum
    };

    static Vector4 createViewportImageRect(
        const Project *project, const Vector4 &viewportLayoutRect) NANOEM_DECL_NOEXCEPT;
    static const char *selectBoneInterpolationType(
        const ITranslator *translator, nanoem_motion_bone_keyframe_interpolation_type_t type) NANOEM_DECL_NOEXCEPT;
    static const char *selectCameraInterpolationType(
        const ITranslator *translator, nanoem_motion_camera_keyframe_interpolation_type_t type) NANOEM_DECL_NOEXCEPT;
    static nanoem_rsize_t findTrack(const void *opaque, ITrack::Type targetTrackType, Project::TrackList &tracks);
    static bool isRhombusReactionSelectable(RhombusReactionType type) NANOEM_DECL_NOEXCEPT;
    static bool handleVectorValueState(IVectorValueState *state);

    ITrack *selectTrack(nanoem_rsize_t offset, ITrack::Type targetTrackType, Project::TrackList &tracks);
    const char *tr(const char *id) NANOEM_DECL_NOEXCEPT;
    RhombusReactionType handleRhombusButton(
        const char *buffer, nanoem_f32_t extent, bool enableDraggingRect) NANOEM_DECL_NOEXCEPT;
    nanoem_f32_t calculateTimelineWidth(const ImVec2 &size);

    void searchNearestKeyframe(nanoem_frame_index_t frameIndex, const Project *project,
        const nanoem_motion_keyframe_object_t *&pk, const nanoem_motion_keyframe_object_t *&nk) NANOEM_DECL_NOEXCEPT;
    void resetBoxSelectionState(Project *project);
    void setCameraLookAt(const Vector3 &value, ICamera *camera, Project *project);
    void setCameraAngle(const Vector3 &value, ICamera *camera, Project *project);
    void setCameraDistance(nanoem_f32_t value, ICamera *camera, Project *project);
    void setCameraFov(nanoem_f32_t value, ICamera *camera, Project *project);
    void setCameraPerspective(bool value, ICamera *camera, Project *project);
    void setLightColor(const Vector3 &value, ILight *light, Project *project);
    void setLightDirection(const Vector3 &value, ILight *light, Project *project);
    void setAccessoryTranslation(const Vector3 &value, Accessory *accessory);
    void setAccessoryOrientation(const Vector3 &value, Accessory *accessory);
    void setAccessoryScaleFactor(nanoem_f32_t value, Accessory *accessory);
    void setAccessoryOpacity(nanoem_f32_t value, Accessory *accessory);
    void setModelBoneTranslation(
        const Vector3 &value, const nanoem_model_bone_t *bonePtr, Model *model, Project *project);
    void setModelBoneOrientation(
        const Vector3 &value, const nanoem_model_bone_t *bonePtr, Model *model, Project *project);
    void setModelMorphWeight(nanoem_f32_t value, const nanoem_model_morph_t *morphPtr, Model *model, Project *project);
    void clearAllKeyframeSelection(Project *project);
    void copyAllKeyframeSelection(const IMotionKeyframeSelection *selection, Project *project);
    void handleDraggingMorphSliderState();
    void setEditingMode(Project *project, Project::EditingMode mode);
    void toggleEditingMode(Project *project, Project::EditingMode mode);
    void drawMainWindow(const Vector2 &devicePixelWindowSize, Project *project, bool &seekable);
    void drawTimeline(nanoem_f32_t timelineWidth, nanoem_f32_t viewportHeight, Project *project);
    void drawSeekerPanel(
        Project *project, nanoem_frame_index_t &frameIndex, bool &frameIndexChanged, bool &forward, bool &backward);
    void drawUndoPanel(Project *project);
    void drawWavePanel(nanoem_f32_t tracksWidth, Project *project, nanoem_u32_t &numVisibleMarkers);
    void drawAllTracksPanel(const ImVec2 &panelSize, Project::TrackList &tracks, Project *project);
    void drawTrack(ITrack *track, int i, Model *activeModel, TrackSet &selectedTracks);
    void handleTrackSelection(Project::TrackList &tracks, Project *project);
    void drawAllMarkersPanel(
        const ImVec2 &panelSize, const Project::TrackList &tracks, nanoem_u32_t numVisibleMarkers, Project *project);
    RhombusReactionType drawMarkerRhombus(nanoem_frame_index_t frameIndex, ITrack *track, nanoem_f32_t extent,
        nanoem_f32_t radius, IMotionKeyframeSelection *source, Project *project);
    void drawKeyframeActionPanel(Project *project);
    void drawKeyframeSelectionPanel(Project *project);
    void drawKeyframeSelectionPanel(void *selector, int index, nanoem_f32_t padding, Project *project);
    void drawViewport(nanoem_f32_t viewportHeight, Project *project);
    void drawViewportParameterBox(Project *project);
    void drawCommonInterpolationControls(Project *project);
    void drawBoneInterpolationPanel(const ImVec2 &panelSize, Model *activeModel, Project *project);
    void drawCameraInterpolationPanel(const ImVec2 &panelSize, Project *project);
    void drawModelPanel(const ImVec2 &panelSize, Project *project);
    void drawCameraPanel(const ImVec2 &panelSize, Project *project);
    void drawLightPanel(const ImVec2 &panelSize, Project *project);
    void drawAccessoryPanel(const ImVec2 &panelSize, Project *project);
    void drawBonePanel(const ImVec2 &panelSize, Model *activeModel, Project *project);
    void drawMorphPanel(const ImVec2 &panelSize, Model *activeModel, Project *project);
    void drawViewPanel(const ImVec2 &panelSize, Project *project);
    void drawPlayPanel(const ImVec2 &panelSize, Project *project);
    void drawAllNonModalWindows(Project *project);
    void drawTransformHandleSet(
        const Vector4UI16 *rects, const ImVec2 &offset, const nanoem_u8_t *icon, int rectType, bool handleable);
    void drawTransformHandleSet(const Project *project, const ImVec2 &offset);
    void drawModelEditingIcon(const Project *project);
    void drawEffectIcon(const Project *project, const ImVec2 &offset);
    void drawFPSCounter(const Project *project, const ImVec2 &offset);
    void drawHardwareMonitor(const Project *project, const ImVec2 &offset);
    void drawBoneTooltip(Project *project);
    void drawTextCentered(const ImVec2 &offset, const Vector4 &rect, const char *text, int length);
    void setupDeviceInput(Project *project);
    void handleVerticalSplitter(
        const ImVec2 &pos, const ImVec2 &size, nanoem_f32_t viewportHeight, nanoem_f32_t devicePixelRatio);
    void handleModalDialogWindow(const Vector2 &devicePixelWindowSize, Project *project);
    void layoutModalDialogWindow(IModalDialog *dialog, Project *project, const Vector2UI16 &devicePixelWindowSize);
    void batchLazySetAllObjects(Project *project, bool seekable);
    void renderDrawList(const Project *project, const ImDrawData *drawData, int sampleCount, Buffer *bufferPtr,
        sg_bindings &bindingsRef, sg::PassBlock &pb);
    void internalFillRect(const Vector4 &devicePixelRect, nanoem_f32_t devicePixelRatio);

    BaseApplicationService *m_applicationPtr;
    ImGuiApplicationMenuBuilder *m_menu;
    Project::IViewportOverlay *m_viewportOverlayPtr;
    CameraLookAtVectorValueState *m_cameraLookAtVectorValueState;
    CameraAngleVectorValueState *m_cameraAngleVectorValueState;
    CameraDistanceVectorValueState *m_cameraDistanceVectorValueState;
    CameraFovVectorValueState *m_cameraFovVectorValueState;
    LightColorVectorValueState *m_lightColorVectorValueState;
    LightDirectionVectorValueState *m_lightDirectionVectorValueState;
    AccessoryTranslationVectorValueState *m_accessoryTranslationVectorValueState;
    AccessoryOrientationVectorValueState *m_accessoryOrientationVectorValueState;
    AccessoryScaleFactorValueState *m_accessoryScaleFactorValueState;
    AccessoryOpacityValueState *m_accessoryOpacityValueState;
    BoneTranslationValueState *m_boneTranslationValueState;
    BoneOrientationValueState *m_boneOrientationValueState;
    DraggingMorphSliderState *m_draggingMorphSliderState;
    ITrack *m_requestedScrollHereTrack;
    PrimitiveContext m_primitive2D;
    ModalDialogList m_allModalDialogs;
    ModalDialogList m_lazyModalDialogs;
    NoModalDialogWindowList m_dialogWindows;
    LazyExecutionCommandList m_lazyExecutionCommands;
    ImFontAtlas m_atlas;
    ImVector<ImWchar> m_textFontRanges;
    ImGuiContext *m_context;
    ImGuiStyle m_style;
    void *m_debugger;
    BufferMap m_buffers;
    PipelineMap m_pipelines;
    Vector4 m_draggingMarkerPanelRect;
    sg_bindings m_bindings;
    sg_pipeline_desc m_basePipelineDescription;
    sg_image m_atlasImage;
    sg_image m_transparentTileImage;
    ScreenCursor m_screenCursor;
    uint64_t m_elapsedTime;
    nanoem_u64_t m_currentMemoryBytes;
    nanoem_u64_t m_maxMemoryBytes;
    nanoem_u32_t m_viewportOverlayFlags;
    nanoem_f32_t m_currentCPUPercentage;
    nanoem_f32_t m_scrollTimelineY;
    nanoem_f32_t m_timelineWidth;
    nanoem_f32_t m_timelineWidthRatio;
    nanoem_f32_t m_lastTimelineWidth;
    nanoem_f32_t m_defaultTimelineWidth;
    int m_movingAllSelectedKeyframesIndexDelta;
    int m_modalDialogPressedButton;
    int m_projectKeyframeSelectorIndex;
    int m_modelKeyframeSelectorIndex;
    bool m_requestClearAllModalDialogs;
    bool m_draggingTimelineScrollBar;
    bool m_lastKeyframeSelection;
    bool m_lastKeyframeCopied;
    bool m_editingAccessoryName;
    bool m_editingModelName;
    bool m_visible;
};

} /* namespace internal */
} /* namespace emapp */

#endif /* NANOEM_EMAPP_INTERNAL_IMGUIWINDOW_H_ */