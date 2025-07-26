#include "imgui.h"

namespace c
{

	inline ImVec4 accent = ImColor(90, 255, 215);

	namespace bg
	{
		inline ImVec4 background = ImColor(27, 29, 34, 255);
		inline ImVec4 shadow = ImColor(0, 0, 0, 40);
		inline ImVec4 border = ImColor(31, 34, 40, 255);
		inline ImVec2 size = ImVec2(810, 540);
		inline float rounding = 16.f;
	}

	namespace child
	{
		inline ImVec4 background = ImColor(31, 34, 40);
		inline ImVec4 stroke = ImColor(41, 46, 55);
		inline ImVec4 cap = ImColor(35, 39, 46);
		inline float rounding = 8.f;
	}

	namespace slider
	{
		inline ImVec4 background = ImColor(36, 40, 48);
		inline ImVec4 scalar = ImColor(255, 255, 255);
	}

	namespace checkbox
	{
		inline ImVec4 background = ImColor(41, 46, 55);
	}

	namespace button
	{
		inline ImVec4 background = ImColor(41, 46, 55);
		inline float rounding = 4.f;
	}


	namespace picker
	{
		inline ImVec4 background = ImColor(41, 46, 55);
		inline float rounding = 4.f;
	}

	namespace input
	{
		inline ImVec4 background = ImColor(27, 30, 34);
		inline float rounding = 4.f;
	}

	
	namespace separator
	{
		inline ImVec4 line = ImColor(41, 46, 55);
	}

	namespace keybind
	{
		inline ImVec4 background = ImColor(36, 40, 48);
		inline float rounding = 2.f;
	}
	
	namespace tabs
	{
		inline ImVec4 background = ImColor(33, 37, 44, 255);
		inline ImVec4 button = ImColor(0, 0, 0, 50);
		inline float rounding = 4.f;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255);
		inline ImVec4 text_hov = ImColor(111, 132, 167);
		inline ImVec4 text = ImColor(72, 87, 113);
	}
}

