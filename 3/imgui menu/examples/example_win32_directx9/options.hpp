#pragma once

#include <map>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <imgui_internal.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#define DIRECTINPUT_VERSION 0x0800

struct ragebot_settings {
	bool enabled = true;
	bool silent = false;
	int priority = 123;
};

class Options
{
public:
	std::map<int, ragebot_settings> ragebot = { };

	ImFont* IconFont;
	ImFont* InterMedium;
};

extern Options opt;