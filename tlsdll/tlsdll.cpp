// tlsdll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

//下面这行告诉链接器在PE文件中要创建TLS目录   
#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used") //引用tlssup.cpp中的_tls_used
#pragma comment (linker, "/INCLUDE:p_tls_callback") //引用下，防止被编译器优化掉（或者去掉/GL优化开关）
#else
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:_p_tls_callback") //引用下，防止被编译器优化掉（或者去掉/GL优化开关）
#endif

/*这是PIMAGE_TLS_CALLBACK()函数的原型，其中第一个和第三个参数保留，第二个参数决定函数在那种情况下*/
/*一共有四个选项DLL_PROCESS_ATTACH、DLL_THREAD_ATTACH、DLL_THREAD_DETACH和DLL_PROCESS_DETACH。详见微软发布的《Microsoft Portable Executable and Common Object File Format Specification v8》*/
void NTAPI dll_tls_callback1(PVOID h, DWORD reason, PVOID pv) {
    if (reason == DLL_PROCESS_ATTACH) {
        OutputDebugStringA("hi,this is dll_tls_callback1, DLL_PROCESS_ATTACH\n"); //crt可能还没初始化，不能调用printf
    } else if (reason == DLL_THREAD_ATTACH) {
        OutputDebugStringA("hi,this is dll_tls_callback1, DLL_THREAD_ATTACH\n");
    }
    return;
}

void NTAPI dll_tls_callback2(PVOID h, DWORD reason, PVOID pv) {
    if (reason == DLL_PROCESS_ATTACH) {
        OutputDebugStringA("hi,this is dll_tls_callback2, DLL_PROCESS_ATTACH\n");
    } else if (reason == DLL_THREAD_ATTACH) {
        OutputDebugStringA("hi,this is dll_tls_callback2, DLL_THREAD_ATTACH\n");
    }
    return;
}

/*
这里使用crt库的tls callback机制，详见tlssup.cpp($(VSInstallDir)VC\crt\src\vcruntime\tlssup.cpp)
tlsup.cpp中定义了：
_CRTALLOC(".CRT$XLA") PIMAGE_TLS_CALLBACK __xl_a = 0;
_CRTALLOC(".CRT$XLZ") PIMAGE_TLS_CALLBACK __xl_z = 0;

由于编译器机制，".CRT$XLB"~".CRT$XLY"节的地址将按顺序分布在".CRT$XLA"和".CRT$XLZ"之间，形成一段连续的地址空间，可以当作一个数组
*/
#pragma const_seg(".CRT$XLB")
EXTERN_C const PIMAGE_TLS_CALLBACK p_tls_callback[] = { dll_tls_callback1, dll_tls_callback2, 0 };
#pragma const_seg()

DWORD WINAPI ThreadProc(
    _In_ LPVOID lpParameter
) {
    OutputDebugStringA("hi,this is ThreadProc\n");
    return 0;
}

void test_thread() {
    HANDLE thread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    WaitForSingleObject(thread, INFINITE);
}