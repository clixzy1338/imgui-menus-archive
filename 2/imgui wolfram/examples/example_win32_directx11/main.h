#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_elements.h"
#include "imgui_freetype.h"
#include <d3d11.h>
#include <tchar.h>

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include <string>
#include <vector>

#include "font.h"
#include "image.h"

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;


std::vector<std::string> words;
char search[24] = { "" };
bool enable_map = true;

static int select_map = 0;
const char* item_map[4]{ "Left corner", "Right corner", "Lower left", "Lower right" };
int size_map = 220;

namespace texture
{
	inline ID3D11ShaderResourceView* background = nullptr;
	inline ID3D11ShaderResourceView* map_one = nullptr;
}

namespace font
{
	inline ImFont* segue_semibold_tabs = nullptr;
	inline ImFont* segue_semibold = nullptr;
	inline ImFont* segue_bold = nullptr;
	inline ImFont* icomoon[14];
}