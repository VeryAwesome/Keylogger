#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#define PATH "Global\\0_VeryAwesome_shared_memory"
#define DLL_PATH "C:\\Users\\Very Awesome\\Documents\\visual studio 2012\\Projects\\hooks\\Debug\\hooks.dll"
#define BUF_SIZE 128

//Really ugly macro to cast a func ptr from dll
#define get_dll_proc(dll_instance, dll_proc_ptr, dll_proc_name, dll_proc_type) \
			dll_proc_ptr = (dll_proc_type)GetProcAddress(dll_instance, dll_proc_name); \
			if(dll_proc_ptr == NULL) {\
			printf("failed loading \"%s\"\n", dll_proc_name); \
			goto done; }

typedef struct cleanun
{
	uint8_t load_dll		: 1;
	uint8_t hook			: 1;

	uint8_t handle_create	: 1;
	uint8_t mm_create		: 1;
}cleanup_t;


typedef int (WINAPI *func_ptr)(int);
typedef int (WINAPI *func_ptr2)();


int temp_counter = 0;
//TODO: Remove prints!
int shared_memory()
{
	int num;
	char * buff = "Hello world from shared memory!";
	HANDLE hMapFile;
	LPCTSTR pBuf;
	cleanup_t clean_flags = {0};
	int retval = 1;
	
	hMapFile = CreateFileMapping(
								INVALID_HANDLE_VALUE,    // use paging file
								NULL,                    // default security
								PAGE_READWRITE,          // read/write access
								0,                       // maximum object size (high-order DWORD)
								BUF_SIZE,                // maximum object size (low-order DWORD)
								PATH);					 // name of mapping object

	if(hMapFile == NULL)
	{
		printf("Cant open file!\n");
		goto done;
	}
	clean_flags.handle_create = 1;

	pBuf = (LPCTSTR)MapViewOfFile(
								hMapFile,				// handle to map object							
								FILE_MAP_ALL_ACCESS,	// read/write permission
								0,
								0,
								BUF_SIZE);

	if(pBuf == NULL)
	{
		printf("Cant open mapping!\n");
		goto done;
	}
	clean_flags.mm_create = 1;
	
	memset((PVOID)pBuf, 0, BUF_SIZE);
	memcpy((PVOID)pBuf, buff, strlen(buff));

	printf("Done!\n");
	scanf("%d", &num);

	retval = 0;


done:
	if(clean_flags.mm_create == 1)
	{
		UnmapViewOfFile(pBuf);
	}
	if(clean_flags.handle_create == 1)
	{
		CloseHandle(hMapFile);
	}
	
	return retval;
}

LRESULT CALLBACK another_hook_proc(int code, WPARAM wParam,  LPARAM lParam)
{
	fopen("C:\\Temp\\bla", "wb");
	return CallNextHookEx(NULL, code, wParam, lParam);	
}

int main()
{
	int temp = 0;
	cleanup_t clean_flags = {0};
	HINSTANCE dll_handler;
	func_ptr2 count_proc, dll_init_proc, can_exit_proc;
	func_ptr set_exit;
	HOOKPROC hook_proc;
	HHOOK hook_handler;
	HMODULE shit_test;

	printf("Starting!\n");

	//Loading dll!
	dll_handler = LoadLibrary(DLL_PATH);
	if(dll_handler == NULL)
	{
		printf("error loading dll!\n");
		goto done;		
	}
	clean_flags.load_dll = 1;

	//Getting dll procs
	get_dll_proc(dll_handler, set_exit, "set_exit", func_ptr);
	get_dll_proc(dll_handler, can_exit_proc, "can_exit", func_ptr2);
	get_dll_proc(dll_handler, count_proc, "get_count", func_ptr2);
	//get_dll_proc(dll_handler, hook_proc, "hook_proc", HOOKPROC);


	//dll_init_proc();
	//do_all_proc();

	/*hook_handler = SetWindowsHookEx(WH_KEYBOARD_LL, hook_proc, dll_handler, 0);
	if(hook_handler == NULL)
	{
		printf("Failed setting hook!\n"); 
		goto done;
	}
	clean_flags.hook = 1;*/

	//hook_proc(0,0,0);

	while (can_exit_proc() == 0)
	{
		printf("write anything except 0 to quit!\n");
		scanf("%d", &temp);
		set_exit(temp);
	}
	set_exit(1);
	printf("Done! %d\n", count_proc());

done:
	/*
	if(clean_flags.hook == 1)
	{
		UnhookWindowsHookEx(hook_handler);
	}*/
	if(clean_flags.load_dll == 1)
	{
		FreeLibrary(dll_handler);
	}
	return 0;
}