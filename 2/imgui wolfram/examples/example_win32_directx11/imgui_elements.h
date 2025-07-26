#define IMGUI_DEFINE_MATH_OPERATORS

#include <cmath>
#include <cstdlib>
#include <imgui.h>
#include <imgui_internal.h>
#include "imgui_settings.h"
#include <map>
#include <windows.h>
#include <iostream>
#include <typeinfo>
#include <map>

namespace font
{
    extern ImFont* icomoon[14];
    extern ImFont* segue_semibold_tabs;
    extern ImFont* segue_semibold;
}

extern char search[24];

namespace ImGui
{

    struct child_state
    {
        float roll;
        bool collapsed;
        float size_offset = 0.f, arrow_roll = 1.57f, alpha;
    };

    bool CustomBeginChildEx(const char* icon, const char* name, ImGuiID id, const ImVec2& size_arg, bool tab_mode, bool open, ImGuiWindowFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;

        flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
        flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);

        const ImVec2 content_avail = GetContentRegionAvail();
        ImVec2 size = ImTrunc(size_arg);
        const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
        if (size.x <= 0.0f)
            size.x = ImMax(content_avail.x + size.x, 4.0f);
        if (size.y <= 0.0f)
            size.y = ImMax(content_avail.y + size.y, 4.0f);

        ImGui::SetCursorPos(GetCursorPos() + ImVec2(0.f, 40.f));

        static std::map<ImGuiID, child_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, child_state() });
            it_anim = anim.find(id);
        }

        GetWindowDrawList()->AddRectFilled(parent_window->DC.CursorPos - ImVec2(0, 40), parent_window->DC.CursorPos + ImVec2(size_arg.x, 0), GetColorU32(c::child::cap), c::child::rounding, ImDrawFlags_RoundCornersTop);
        GetWindowDrawList()->AddRectFilled(parent_window->DC.CursorPos + ImVec2(1, -1), parent_window->DC.CursorPos + ImVec2(size_arg.x, 0), GetColorU32(c::child::stroke), 0.f);

            PushClipRect(parent_window->DC.CursorPos + ImVec2(0, -40), parent_window->DC.CursorPos + ImVec2(size_arg.x, 0), true);
            GetWindowDrawList()->AddText(parent_window->DC.CursorPos + ImVec2(tab_mode ? (40 - CalcTextSize(icon).y) : 75, (-40 - CalcTextSize(name).y) + 1) / 2, GetColorU32(c::text::text_active), name);
            PopClipRect();

            PushFont(font::icomoon[15]);
            GetWindowDrawList()->AddText(parent_window->DC.CursorPos + ImVec2(tab_mode ? (size_arg.x - (40 - CalcTextSize("i").y)) * 2 : 40 - CalcTextSize(icon).y, (-40 - CalcTextSize(tab_mode ? "i" : icon).y)) / 2, GetColorU32(tab_mode ? c::text::text : c::accent), tab_mode ? "i" : icon);
            PopFont();

        const char* temp_window_name;
        if (name)
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
        else
            ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

        bool ret = Begin(temp_window_name, NULL, flags | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImGuiWindow* child_window = g.CurrentWindow;
        child_window->ChildId = id;
        child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

        bool panel_hovered, panel_clicked = false;

        panel_hovered = g.IO.MousePos.x > parent_window->DC.CursorPos.x + size_arg.x - 50 && g.IO.MousePos.y > parent_window->DC.CursorPos.y - 35 && g.IO.MousePos.x < parent_window->DC.CursorPos.x + size.x && g.IO.MousePos.y < parent_window->DC.CursorPos.y;
        panel_clicked = panel_hovered && g.IO.MouseClicked[0];

        if (panel_clicked && open) it_anim->second.collapsed = !it_anim->second.collapsed;

        if (child_window->BeginCount == 1) parent_window->DC.CursorPos = child_window->Pos;

        if (tab_mode)
        it_anim->second.size_offset = ImLerp(it_anim->second.size_offset, it_anim->second.collapsed ? child_window->ContentSize.y + 29 : 1.f, g.IO.DeltaTime * 10.f);
        else
        it_anim->second.size_offset = ImLerp(it_anim->second.size_offset, child_window->ContentSize.y > 0 ? child_window->ContentSize.y + 29 : 1.f, g.IO.DeltaTime * 10.f);

        SetWindowSize(ImVec2(size.x, it_anim->second.size_offset));

        parent_window->DrawList->AddRectFilled(child_window->Pos, child_window->Pos + ImVec2(size_arg.x, it_anim->second.size_offset), GetColorU32(tab_mode ? c::tabs::background : c::child::background), c::child::rounding, ImDrawFlags_RoundCornersBottom);

        if (child_window->BeginCount == 1) parent_window->DC.CursorPos = child_window->Pos;

        const ImGuiID temp_id_for_activation = ImHashStr("##Child", 0, id);
        if (g.ActiveId == temp_id_for_activation)
            ClearActiveID();
        if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY))
        {
            FocusWindow(child_window);
            NavInitWindow(child_window, false);
            SetActiveID(temp_id_for_activation, child_window);
            g.ActiveIdSource = g.NavInputSource;
        }
        return ret;
    }

    bool CustomBeginChild(const char* icon, ImGuiID id, const ImVec2& size_arg, bool tab_mode, bool open, ImGuiWindowFlags extra_flags)
    {
        IM_ASSERT(id != 0);
        return CustomBeginChildEx(icon, NULL, id, size_arg, tab_mode, open, extra_flags);
    }

    bool CustomBeginChild(const char* icon, const char* str_id, const ImVec2& size_arg, bool tab_mode, bool open, ImGuiWindowFlags extra_flags)
    {
        ImGuiWindow* window = GetCurrentWindow();

        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 15));
        return CustomBeginChildEx(icon, str_id, window->GetID(str_id), size_arg, tab_mode, open, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding);
    }

    void CustomEndChild()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        PopStyleVar(2);
        IM_ASSERT(g.WithinEndChild == false);
        IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

        g.WithinEndChild = true;
        if (window->BeginCount > 1)
        {
            End();
        }
        else
        {
            ImVec2 sz = window->Size;
            if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
                sz.x = ImMax(4.0f, sz.x);
            if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
                sz.y = ImMax(4.0f, sz.y);
            End();

            ImGuiWindow* parent_window = g.CurrentWindow;
            ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
            ItemSize(sz - ImVec2(0, 0));

             if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavWindowHasScrollY) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
            {
                ItemAdd(bb, window->ChildId);
                RenderNavHighlight(bb, window->ChildId);

                // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
                if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
                    RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
            }
            else
            {
                // Not navigable into
                ItemAdd(bb, 0);

                // But when flattened we directly reach items, adjust active layer mask accordingly
                if (window->Flags & ImGuiWindowFlags_NavFlattened)
                    parent_window->DC.NavLayersActiveMaskNext |= window->DC.NavLayersActiveMaskNext;
            }
            if (g.HoveredWindow == window)
                g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
        }
        g.WithinEndChild = false;
        g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
    }


    struct slider_state {
        float plus_float;
        int plus_int;
        ImVec4 background, circle, text;
        float slow_anim, circle_anim;
        float position;
    };


    bool KnobScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = GetContentRegionMax().x - style.WindowPadding.x;

        std::string s = search;

        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        const ImRect frame_bb(window->DC.CursorPos + ImVec2(0, 0), window->DC.CursorPos + ImVec2(w, 26.f));

        const ImRect slider_bb(window->DC.CursorPos + ImVec2(w - 30, 0), window->DC.CursorPos + ImVec2(w, 100));

        const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? label_size.x : 0.0f, 0.0f));

        const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
        ItemSize(ImRect(total_bb.Min, total_bb.Max - ImVec2(0, 0)));

        if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0)) return false;

        if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

        const bool hovered = ItemHoverable(frame_bb, id, NULL);
        bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
        if (!temp_input_is_active)
        {
            const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
            const bool clicked = hovered && IsMouseClicked(0, id);
            const bool make_active = (input_requested_by_tabbing || clicked || g.NavActivateId == id);
            if (make_active && clicked)
                SetKeyOwner(ImGuiKey_MouseLeft, id);
            if (make_active && temp_input_allowed)
                if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                    temp_input_is_active = true;

            if (make_active && !temp_input_is_active)
            {
                SetActiveID(id, window);
                SetFocusID(id, window);
                FocusWindow(window);
                g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
            }
        }

        ImRect grab_bb;

        static std::map<ImGuiID, slider_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, slider_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.circle_anim = ImLerp(it_anim->second.circle_anim, IsItemActive() ? 11.f : 10.f, g.IO.DeltaTime * 6.f);

        if ((flags & ImGuiSliderFlags_Integer) == 0) {
            it_anim->second.plus_float = ImLerp(it_anim->second.plus_float, *(float*)p_data <= *(float*)p_max && hovered && GetAsyncKeyState(VK_OEM_PLUS) & 0x01 ? *(float*)p_data += 0.05f : *(float*)p_data >= *(float*)p_min && hovered && GetAsyncKeyState(VK_OEM_MINUS) & 0x01 ? *(float*)p_data -= 0.05f : 0, g.IO.DeltaTime * 6.f);
            if (*(float*)p_data > *(float*)p_max) *(float*)p_data = *(float*)p_max;
            if (*(float*)p_data < *(float*)p_min) *(float*)p_data = *(float*)p_min;
        }
        else
        {
            it_anim->second.plus_int = ImLerp(it_anim->second.plus_int, *(int*)p_data <= *(int*)p_max && hovered && GetAsyncKeyState(VK_OEM_PLUS) & 0x01 ? *(int*)p_data += 1 : *(int*)p_data >= *(int*)p_min && hovered && GetAsyncKeyState(VK_OEM_MINUS) & 0x01 ? *(int*)p_data -= 1 : 0, g.IO.DeltaTime * 6.f);
            if (*(int*)p_data > *(int*)p_max) *(int*)p_data = *(int*)p_max;
            if (*(int*)p_data < *(int*)p_min) *(int*)p_data = *(int*)p_min;
        }

        it_anim->second.text = ImLerp(it_anim->second.text, g.ActiveId == id || s == label ? s == label ? c::accent : c::text::text_active : hovered ? c::text::text_hov : c::text::text, g.IO.DeltaTime * 6.f);

        const bool value_changed = DragBehavior(id, data_type, p_data, 0.f, p_min, p_max, format, NULL);


        if (value_changed) MarkItemEdited(id);

        char value_buf[64];
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        static float radius = 10.f;
        static float thickness = 3.5f;

        it_anim->second.position = ImLerp(it_anim->second.position, *static_cast<float*>(p_data) / *reinterpret_cast<const float*>(p_max) * 6.25f, ImGui::GetIO().DeltaTime * 18.f);

        GetWindowDrawList()->PathClear();
        GetWindowDrawList()->PathArcTo(ImVec2(frame_bb.Max.x + radius - 23, frame_bb.Min.y + (26 / 2)), radius, 0.f, 2.f * IM_PI, 40.f);
        GetWindowDrawList()->PathStroke(GetColorU32(c::slider::background), 0, thickness);

        GetWindowDrawList()->PathClear(); 
        GetWindowDrawList()->PathArcTo(ImVec2(frame_bb.Max.x + radius - 23, frame_bb.Min.y + (26 / 2)), radius, IM_PI * 1.5f, IM_PI * 1.5f + it_anim->second.position, 40.f);
        GetWindowDrawList()->PathStroke(GetColorU32(c::accent), 0, thickness);

        GetWindowDrawList()->AddCircleFilled(ImVec2(frame_bb.Max.x + radius - 22.5f + ImCos(IM_PI * 1.5f + it_anim->second.position) * radius, frame_bb.Min.y + (30 / 2) + ImSin(IM_PI * 1.5f + it_anim->second.position) * radius - 1.5), 3.f, GetColorU32(c::slider::scalar));

        GetWindowDrawList()->AddText(ImVec2(frame_bb.Max.x - (40 + CalcTextSize(value_buf).x), frame_bb.Min.y + (26 - CalcTextSize(value_buf).y) / 2), GetColorU32(c::accent, 0.3f), value_buf);

        GetWindowDrawList()->AddText(ImVec2(frame_bb.Max.x - w, frame_bb.Min.y + (26 - CalcTextSize(value_buf).y) / 2), GetColorU32(it_anim->second.text), label);

        return value_changed;
    }

    bool KnobFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        return KnobScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
    }

    bool KnobInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        return KnobScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags | ImGuiSliderFlags_Integer);
    }







    struct tab_state
    {
        ImVec4 text;
        float alpha;
    };

    bool Tab(bool selected, const char* label, const ImVec2& size_arg)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;

        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

        const ImRect bb(pos, pos + size);
        ItemSize(ImRect(bb.Min, bb.Max - ImVec2(0, 0)));
        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, NULL);

        static std::map<ImGuiID, tab_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, tab_state() });
            it_anim = anim.find(id);
        }
        
        it_anim->second.text = ImLerp(it_anim->second.text, selected ? c::text::text_active : hovered ? c::text::text_hov : c::text::text, g.IO.DeltaTime * 6.f);
        it_anim->second.alpha = ImLerp(it_anim->second.alpha, selected ? 1.f : 0.f, g.IO.DeltaTime * 6.f);

        GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(c::tabs::button, it_anim->second.alpha), c::tabs::rounding);

        GetWindowDrawList()->AddCircleFilled(bb.Min + ImVec2(size_arg.x - size_arg.y / 2, size_arg.y / 2), 3.f, GetColorU32(c::accent, it_anim->second.alpha), 100);

        PushFont(font::segue_semibold_tabs);
        GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (size.y - CalcTextSize(label).y) / 2, bb.Max.y - CalcTextSize(label).y - (size.y - CalcTextSize(label).y) / 2 - 1), GetColorU32(it_anim->second.text), label);
        PopFont();

        return pressed;
    }

    struct button_state
    {
        ImVec4 background, text;
    };

    bool CustomButton(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();

        if (window->SkipItems) return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true), pos = window->DC.CursorPos;

        ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

        const ImRect bb(pos, pos + size);

        ItemSize(size, 0.f);
        if (!ItemAdd(bb, id)) return false;

        bool hovered, held, pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

        static std::map<ImGuiID, button_state> anim;
        auto it_anim = anim.find(id);

        if (it_anim == anim.end())
        {
            anim.insert({ id, button_state() });
            it_anim = anim.find(id);
        }

        it_anim->second.background = ImLerp(it_anim->second.background, IsItemActive() ? c::accent : c::button::background, g.IO.DeltaTime * 6.f);

        ImGui::GetWindowDrawList()->AddRectFilled(bb.Min, bb.Max, GetColorU32(it_anim->second.background), c::button::rounding);

        ImGui::GetWindowDrawList()->AddText(ImVec2(bb.Min.x + (size_arg.x - CalcTextSize(label).x) / 2, bb.Max.y - CalcTextSize(label).y - (size.y - CalcTextSize(label).y) / 2 - 1), GetColorU32(c::text::text_active), label);

        return pressed;
    }

}