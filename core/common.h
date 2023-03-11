#pragma once

/* winapi */
#include <windows.h>

/* common library */
#include <cmath>
#include <string>

/* directx includes */
#include <d3d9.h>
#include <d3dx9.h>

/* imgui */
// @credits: https://github.com/ocornut/imgui
#include "../ext/imgui/imgui.h"
#include "../ext/imgui/freetype/imgui_freetype.h"

// math ImVec2 operator
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ext/imgui/imgui_internal.h"

// backend
#include "../ext/imgui/dx9/imgui_impl_dx9.h"
#include "../ext/imgui/win32/imgui_impl_win32.h"

// used: time_t, tm, std::time
#include <chrono>

// used: ns,ms,s,m,h time literals
using namespace std::chrono_literals;

// used: xor_str encryption
#include "utilities/xor_str.h"