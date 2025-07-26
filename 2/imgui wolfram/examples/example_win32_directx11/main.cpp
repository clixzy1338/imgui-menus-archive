#include "main.h"

int main(int, char**)
{
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor | ImGuiFreeTypeBuilderFlags_Bitmap;

    font::segue_semibold = io.Fonts->AddFontFromMemoryTTF(segue_semibold, sizeof(segue_semibold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::segue_semibold_tabs = io.Fonts->AddFontFromMemoryTTF(segue_semibold, sizeof(segue_semibold), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::segue_bold = io.Fonts->AddFontFromMemoryTTF(segue_bold, sizeof(segue_bold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    for (int i = 15; i < 25; i++) font::icomoon[i] = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), i, &cfg, io.Fonts->GetGlyphRangesCyrillic());


    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
    if (texture::background == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &info, pump, &texture::background, 0);
    if (texture::map_one == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, map_one, sizeof(map_one), &info, pump, &texture::map_one, 0);

    bool done = false;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done) break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiStyle* style = &ImGui::GetStyle();

            static float color[4] = { 124 / 255.f, 103 / 255.f, 255 / 255.f, 1.f };
            c::accent = { color[0], color[1], color[2], 1.f };

            style->WindowPadding = ImVec2(0, 0);
            style->ItemSpacing = ImVec2(20, 20);
            style->WindowBorderSize = 0;
            style->ScrollbarSize = 6.f;

            ImGui::GetBackgroundDrawList()->AddImage(texture::background, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

            ImGui::SetNextWindowSizeConstraints(ImVec2(c::bg::size), ImGui::GetIO().DisplaySize);

            ImGui::Begin("IMGUI", nullptr, window_flags);
            {
                const ImVec2& pos = ImGui::GetWindowPos();
                const ImVec2& region = ImGui::GetContentRegionMax();
                const ImVec2& spacing = style->ItemSpacing;

                ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(region), ImGui::GetColorU32(c::bg::background), c::bg::rounding);

                ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(180, region.y), ImGui::GetColorU32(c::bg::border), c::bg::rounding, ImDrawFlags_RoundCornersLeft);
                ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(180, 0), pos + ImVec2(region.x, 50), ImGui::GetColorU32(c::bg::border), c::bg::rounding, ImDrawFlags_RoundCornersTopRight);

                ImGui::GetWindowDrawList()->AddRectFilledMultiColor(pos + ImVec2(0, 50), pos + ImVec2(region.x, 65), ImGui::GetColorU32(c::bg::shadow), ImGui::GetColorU32(c::bg::shadow), ImGui::GetColorU32(c::bg::shadow, 0.f), ImGui::GetColorU32(c::bg::shadow, 0.f));
                ImGui::GetWindowDrawList()->AddRectFilledMultiColor(pos + ImVec2(180, 50), pos + ImVec2(195, region.y), ImGui::GetColorU32(c::bg::shadow), ImGui::GetColorU32(c::bg::shadow, 0.f), ImGui::GetColorU32(c::bg::shadow, 0.f), ImGui::GetColorU32(c::bg::shadow));

                ImGui::PushFont(font::segue_bold);
                ImGui::GetWindowDrawList()->AddText(pos + ImVec2(50 - ImGui::CalcTextSize("RavenCheats").y, 50 - ImGui::CalcTextSize("RavenCheats").y) / 2, ImGui::GetColorU32(c::accent), "RavenCheats");
                ImGui::GetWindowDrawList()->AddText(pos + ImVec2(region.x - ((195 + 64) + (35 / 2) + ImGui::CalcTextSize("build 20.01.2023").x), (50 - ImGui::CalcTextSize("build 20.01.2023").y) / 2), ImGui::GetColorU32(c::accent, 0.2f), "build 20.01.2023");
                ImGui::PopFont();

                ImGui::PushFont(font::icomoon[18]);
                ImGui::GetWindowDrawList()->AddText(pos + ImVec2(region.x - (54 - ImGui::CalcTextSize("j").y), (50 - ImGui::CalcTextSize("j").y) / 2), ImGui::GetColorU32(c::accent), "j");
                ImGui::PopFont();

                ImGui::SetCursorPos(ImVec2(region.x - (195 + 57), (50 - 38) / 2));
                ImGui::InputTextEx("m", "SEARCH", search, ARRAYSIZE(search), ImVec2(195, 40), NULL);

                ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(region.x - 53, 0), pos + ImVec2(region.x - 50, 50), ImGui::GetColorU32(c::bg::background), 0.f);
                ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(region.x - (195 + 64), 0), pos + ImVec2(region.x - (195 + 61), 50), ImGui::GetColorU32(c::bg::background), 0.f);

                static int tabs = 0;

                ImGui::SetCursorPos(ImVec2(0, 50) + spacing);
                ImGui::BeginChild("JUST TAB", ImVec2(180, region.y - (50 + spacing.y)) - spacing, NULL);
                {

                    ImGui::CustomBeginChild("d", "Aimbot", ImVec2((180 - spacing.x * 2), 0), true, true, NULL);
                    {

                        if (ImGui::Tab(0 == tabs, "General", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 0;

                        if (ImGui::Tab(1 == tabs, "Triggerbot", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 1;

                        if (ImGui::Tab(2 == tabs, "Miscellaneous", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 2;

                    }
                    ImGui::CustomEndChild();

                    ImGui::CustomBeginChild("d", "Visuals", ImVec2((180 - spacing.x * 2), 0), true, true, NULL);
                    {

                        if (ImGui::Tab(3 == tabs, "Players", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 3;

                        if (ImGui::Tab(4 == tabs, "Grenades", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 4;

                        if (ImGui::Tab(5 == tabs, "Inventory", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 5;

                    }
                    ImGui::CustomEndChild();

                    ImGui::CustomBeginChild("d", "Misc", ImVec2((180 - spacing.x * 2), 0), true, true, NULL);
                    {

                        if (ImGui::Tab(6 == tabs, "World", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 6;

                        if (ImGui::Tab(7 == tabs, "Scripts", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 7;

                        if (ImGui::Tab(8 == tabs, "SkinChanger", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 8;

                    }
                    ImGui::CustomEndChild();

                    ImGui::CustomBeginChild("d", "Settings", ImVec2((180 - spacing.x * 2), 0), true, true, NULL);
                    {

                        if (ImGui::Tab(9 == tabs, "Config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 9;

                        if (ImGui::Tab(10 == tabs, "Interface", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 10;

                        if (ImGui::Tab(11 == tabs, "Miscellaneous", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30))) tabs = 11;

                    }
                    ImGui::CustomEndChild();

                }
                ImGui::EndChild();

                static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

                ImGui::SetCursorPos(ImVec2(180, 50) + spacing);

                ImGui::BeginChild("JUST CHILD", ImVec2(region.x - 180, region.y - (50 + spacing.y)) - spacing);
                {

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * ImGui::GetStyle().Alpha);

                    if (active_tab == 0) {

                        ImGui::BeginGroup();
                        {

                            ImGui::CustomBeginChild("c", "General", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                static bool enable = true;
                                ImGui::Checkbox("Enable aimbot", &enable);

                                ImGui::Separator();

                                static bool while_flashed = false;
                                ImGui::Checkbox("disable aimbot while flashed", &while_flashed);

                                ImGui::Separator();

                                static bool disable = true;
                                ImGui::Checkbox("disable aimbot through smoke", &disable);

                                ImGui::Separator();

                                static char input[64] = { "" };
                                ImGui::InputTextEx("x", "Enter anything here", input, ARRAYSIZE(input), ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40), NULL);


                            }
                            ImGui::CustomEndChild();

                            ImGui::CustomBeginChild("h", "Others", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                static float asix_x = 0.850f;
                                ImGui::KnobFloat("X Asix", &asix_x, 0.000f, 1.000f, "%.3fF", NULL);

                                ImGui::Separator();

                                static float asix_y = 0.350f;
                                ImGui::KnobFloat("Y Asix", &asix_y, 0.000f, 1.000f, "%.3fF", NULL);

                                ImGui::Separator();

                                static char input[64] = { "" };
                                ImGui::InputTextEx("x", "Enter anything here", input, ARRAYSIZE(input), ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40), NULL);

                            }
                            ImGui::CustomEndChild();

                            ImGui::CustomBeginChild("e", "Exploits", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                if (ImGui::CustomButton("Button", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL));

                                ImGui::Separator();

                                ImGui::CustomBeginChild("d", "Open me up", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 0), true, true, NULL);
                                {

                                    ImGui::CustomBeginChild("d", "How do you like me?", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 0), true, true, NULL);
                                    {

                                        bool check_test[15];

                                        for (int i = 50; i <= 55; i++)
                                        {
                                            std::string s = std::to_string(i);
                                            ImGui::Checkbox(s.c_str(), &check_test[i]);

                                            if (i != 55) ImGui::Separator();

                                        }

                                    }
                                    ImGui::CustomEndChild();

                                    ImGui::CustomBeginChild("d", "I'm just here", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 0), true, true, NULL);
                                    {

                                        bool check_test[15];

                                        for (int i = 50; i <= 55; i++)
                                        {
                                            std::string s = std::to_string(i);
                                            ImGui::Checkbox(s.c_str(), &check_test[i]);

                                            if (i != 55) ImGui::Separator();

                                        }

                                    }
                                    ImGui::CustomEndChild();

                                    ImGui::CustomBeginChild("d", "I was made by Past Owl", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 0), true, true, NULL);
                                    {

                                        bool check_test[15];

                                        for (int i = 50; i <= 55; i++)
                                        {
                                            std::string s = std::to_string(i);
                                            ImGui::Checkbox(s.c_str(), &check_test[i]);

                                            if (i != 55) ImGui::Separator();

                                        }

                                    }
                                    ImGui::CustomEndChild();

                                    ImGui::Separator();

                                    if (ImGui::CustomButton("Oh $h1t that button again", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL));

                                }
                                ImGui::CustomEndChild();

                            }
                            ImGui::CustomEndChild();

                        }
                        ImGui::EndGroup();

                        ImGui::SameLine();

                        ImGui::BeginGroup();
                        {

                            ImGui::CustomBeginChild("l", "Miscellaneous", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                static bool multi_num[5] = { false, true, true, true, false };
                                const char* multi_items[5] = { "Head", "Chest", "Body", "Legs", "Neck" };
                                ImGui::MultiCombo("Hitbox priority", multi_num, multi_items, 5);

                                ImGui::Separator();

                                ImGui::ColorEdit4("Dot color", color, picker_flags);

                                ImGui::Separator();

                                static int speed_yaw = 75;
                                ImGui::KnobInt("Speed (yaw)", &speed_yaw, 0, 100, "%d%%", NULL);

                                ImGui::Separator();

                                static int speed_pitch = 75;
                                ImGui::KnobInt("Speed (pitch)", &speed_pitch, 0, 100, "%d%%", NULL);

                            }
                            ImGui::CustomEndChild();

                            ImGui::CustomBeginChild("b", "Keybinds", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                static int key = 0;
                                ImGui::Keybind("Keybind", &key);

                                ImGui::Separator();

                                static int select = 0;
                                const char* items[2]{ "Default", "Interaction" };
                                ImGui::Combo("Safe points", &select, items, IM_ARRAYSIZE(items), ARRAYSIZE(items));

                                ImGui::Separator();

                                if (ImGui::CustomButton("Button", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL));

                            }
                            ImGui::CustomEndChild();

                        }
                        ImGui::EndGroup();

                    }
                    else if (active_tab == 9)
                    {

                        static char cfg_create[24] = { "" };

                        ImGui::BeginGroup();
                        {

                            ImGui::CustomBeginChild("g", "Create", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                ImGui::InputTextEx("x", "CONFIG CREATE", cfg_create, ARRAYSIZE(cfg_create), ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 40), NULL);

                                ImGui::Separator();

                                if (ImGui::CustomButton("Create a config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL))
                                {
                                    words.push_back(cfg_create);
                                };

                            }
                            ImGui::CustomEndChild();

                            ImGui::CustomBeginChild("o", "Config list", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                static int cfg_selector = 0;

                                ImGui::CustomBeginChild("d", "Legit category", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 0), true, true, NULL);
                                {

                                    if (ImGui::Tab(0 == cfg_selector, "Visuals and skins", ImVec2(ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x, 30))) cfg_selector = 0;

                                    if (ImGui::Tab(1 == cfg_selector, "My ESP Settings", ImVec2(ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x, 30))) cfg_selector = 1;

                                    if (ImGui::Tab(2 == cfg_selector, "Something legit...", ImVec2(ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x, 30))) cfg_selector = 2;

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("d", "Rage category", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 0), true, true, NULL);
                                {

                                    if (ImGui::Tab(3 == cfg_selector, "Crazy aimbot settings", ImVec2(ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x, 30))) cfg_selector = 3;

                                    if (ImGui::Tab(4 == cfg_selector, "Anti aim and other entertainment", ImVec2(ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x, 30))) cfg_selector = 4;

                                    if (ImGui::Tab(5 == cfg_selector, "Strafe and trigger", ImVec2(ImGui::GetContentRegionMax().x - ImGui::GetStyle().WindowPadding.x, 30))) cfg_selector = 5;

                                }
                                ImGui::CustomEndChild();


                            }
                            ImGui::CustomEndChild();

                        }
                        ImGui::EndGroup();

                        ImGui::SameLine();

                        ImGui::BeginGroup();
                        {

                            ImGui::CustomBeginChild("t", "Information", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                ImGui::TextColored(ImColor(ImGui::GetColorU32(c::text::text_active)), "Author");
                                ImGui::SameLine(0, ImGui::GetContentRegionMax().x - (ImGui::CalcTextSize("Past Owl").x + ImGui::CalcTextSize("Author").x + ImGui::GetStyle().WindowPadding.x));
                                ImGui::TextColored(ImColor(ImGui::GetColorU32(c::accent)), "Past Owl");

                                ImGui::Separator();

                                ImGui::TextColored(ImColor(ImGui::GetColorU32(c::text::text_active)), "Created at");
                                ImGui::SameLine(0, ImGui::GetContentRegionMax().x - (ImGui::CalcTextSize("20.10.2023 - 15:00").x + ImGui::CalcTextSize("Created at").x + ImGui::GetStyle().WindowPadding.x));
                                ImGui::TextColored(ImColor(ImGui::GetColorU32(c::accent)), "20.10.2023 - 15:00");

                                ImGui::Separator();

                                ImGui::TextColored(ImColor(ImGui::GetColorU32(c::text::text_active)), "Modified at");
                                ImGui::SameLine(0, ImGui::GetContentRegionMax().x - (ImGui::CalcTextSize("22.10.2023 - 17:30").x + ImGui::CalcTextSize("Modified at").x + ImGui::GetStyle().WindowPadding.x));
                                ImGui::TextColored(ImColor(ImGui::GetColorU32(c::accent)), "22.10.2023 - 17:30");

                            }
                            ImGui::CustomEndChild();

                            ImGui::CustomBeginChild("r", "Integration", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                            {

                                if (ImGui::CustomButton("LOAD", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL));

                                if (ImGui::CustomButton("SAVE", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL));

                                ImGui::Separator();

                                if (ImGui::CustomButton("RESET", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL));

                                if (ImGui::CustomButton("DELETE", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 30), NULL));

                            }
                            ImGui::CustomEndChild();

                        }
                        ImGui::EndGroup();

                    }
                    else if (active_tab == 6)
                    {

                        ImGui::CustomBeginChild("l", "Mini-Map", ImVec2((region.x - (180 + spacing.x * 3)) / 2, 0), false, NULL, NULL);
                        {

                            ImGui::Checkbox("Enable Mini-Map", &enable_map);

                            ImGui::Separator();

                            ImGui::Combo("Mini-Map position", &select_map, item_map, IM_ARRAYSIZE(item_map), ARRAYSIZE(item_map));

                            ImGui::Separator();

                            ImGui::KnobInt("Mini-Map size", &size_map, 100, 250, NULL, NULL);
                        }
                        ImGui::CustomEndChild();

                    }

                    ImGui::PopStyleVar();

                }
                ImGui::EndChild();

            }
            ImGui::End();

            const ImVec2 map_size = ImVec2(size_map, size_map);

            const ImVec2 map_position[4] = { ImVec2(25, 25), ImVec2(GetSystemMetrics(SM_CXSCREEN) - (map_size.x + 25), 25), ImVec2(25, GetSystemMetrics(SM_CYSCREEN) - (map_size.y + 25)), ImVec2(GetSystemMetrics(SM_CXSCREEN) - (map_size.x + 25), GetSystemMetrics(SM_CYSCREEN) - (map_size.y + 25)) };
            

            if (enable_map) {
            ImGui::SetNextWindowSize(ImVec2(map_size));
            ImGui::SetNextWindowPos(ImVec2(map_position[select_map]));

                ImGui::Begin("MINI-MAP", nullptr, window_flags | ImGuiWindowFlags_NoDecoration);
                {
                    const ImVec2& pos = ImGui::GetWindowPos();

                    ImGui::GetBackgroundDrawList()->AddImage(texture::map_one, pos, pos + ImGui::GetContentRegionMax(), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

                }
                ImGui::End();
            }

        }
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);

    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
