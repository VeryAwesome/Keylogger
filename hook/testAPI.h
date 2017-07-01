#ifndef _TEST_API_H_
#define _TEST_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <Windows.h>
#include <stdio.h>

__declspec(dllimport) int __stdcall get_count();

int __stdcall set_exit(int exit);

int __stdcall can_exit();

LRESULT __declspec(dllimport) CALLBACK hook_proc(int code, WPARAM wParam,  LPARAM lParam);

int __stdcall get_last_key();

#ifdef __cplusplus
}
#endif

#endif