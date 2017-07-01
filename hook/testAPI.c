#include "testAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "d_queue.h"

#define TEMP_FILE "C:\\Users\\Very Awesome\\Documents\\temp\\bla.bli"

static int num = 0, volatile exit_flag = 0, open_count = 0;

static HHOOK hook_handler = NULL;
static HINSTANCE dll_handler;
static HANDLE tmpHandle = NULL;
static FILE * f = NULL;

BOOL CALLBACK do_all();
static void clean_file();
static void clean_hook();


int APIENTRY DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if(open_count == 0)
			{
				init_queue();
				f = fopen(TEMP_FILE, "wb+");
				if(hInstance != NULL)
				{
					dll_handler = hInstance;
					//printf("is not NULL!\n");
				}
				tmpHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&do_all, 0, 0, 0);

				//if(tmpHandle == NULL)
					//printf("Failed creating thread!\n");
			}
			open_count ++;
		}
		break;
		
		case DLL_PROCESS_DETACH:
		{
			if(open_count == 1)
			{
				clean_hook();
				clean_file();
				destroy_queue();
			}
			open_count--;
		}
		break;

		default:
			break;
	}
	return TRUE;

}

__declspec(dllexport) int __stdcall get_count()
{
	return num;
}

int __stdcall set_exit(int flag)
{
	exit_flag = flag;
	return flag;

}

int __stdcall can_exit()
{
	return exit_flag;
}

LRESULT __declspec(dllexport)__stdcall CALLBACK hook_proc(int code, WPARAM wParam,  LPARAM lParam)
{
	if(code == HC_ACTION && (lParam & 0x80000000) == 0)
	{
		if(wParam > 0x7)
		{
			push(wParam);
			num++;
		}
	}
	return CallNextHookEx(hook_handler, code, wParam, lParam);	
}

int __stdcall get_last_key()
{
	return (int)(pop() &0xff);
}

BOOL CALLBACK do_all()
{
	MSG msg;
	BOOL bRet;

	//printf("dll_handler = %d\n", dll_handler);

	hook_handler = SetWindowsHookEx(WH_KEYBOARD, hook_proc, dll_handler, 0);
	if(hook_handler == NULL)
	{
		//printf("failed T^T\n");
		exit_flag = 1;
		return TRUE;
	}
		
	while(exit_flag == 0 && (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}

	exit_flag = 1;
	
	clean_hook();
	clean_file();

	return TRUE;

}

static void clean_file()
{
	if(f != NULL)
		fclose(f);
	f = NULL;
}

static void clean_hook()
{
	if(hook_handler != NULL)
	{
		UnhookWindowsHookEx(hook_handler);
	}
	hook_handler = NULL;
}

#ifdef __cplusplus
}
#endif