#pragma once

#include <windows.h>
#include <stdio.h>
#pragma warning(disable:4996)

#define Assert(x) if (!x) { MessageBoxA(0, #x, "Assertion Failed", MB_OK | MB_ICONERROR); __debugbreak(); }
#define Check(x) if (!x) { MessageBoxA(0, #x, "Check Failed", MB_OK | MB_ICONERROR); __debugbreak(); }

#define Assert_(x, msg) if (!x) { MessageBoxA(0, msg, "Assertion Failed", MB_OK | MB_ICONERROR); __debugbreak(); }
#define Check_(x, msg) if (!x) { MessageBoxA(0, msg, "Check Failed", MB_OK | MB_ICONERROR); __debugbreak(); }

#ifdef _DEBUG
#define ErrMsg(msg) MessageBoxA(0, msg, "Error", MB_OK | MB_ICONERROR);
#else
#define ErrMsg(msg)
#endif

#include "bpd-dll-import.h"
#include "bpd-window.h"