// tlsdll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

//�������и�����������PE�ļ���Ҫ����TLSĿ¼   
#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used") //����tlssup.cpp�е�_tls_used
#pragma comment (linker, "/INCLUDE:p_tls_callback") //�����£���ֹ���������Ż���������ȥ��/GL�Ż����أ�
#else
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:_p_tls_callback") //�����£���ֹ���������Ż���������ȥ��/GL�Ż����أ�
#endif

/*����PIMAGE_TLS_CALLBACK()������ԭ�ͣ����е�һ���͵����������������ڶ��������������������������*/
/*һ�����ĸ�ѡ��DLL_PROCESS_ATTACH��DLL_THREAD_ATTACH��DLL_THREAD_DETACH��DLL_PROCESS_DETACH�����΢�����ġ�Microsoft Portable Executable and Common Object File Format Specification v8��*/
void NTAPI dll_tls_callback1(PVOID h, DWORD reason, PVOID pv) {
    if (reason == DLL_PROCESS_ATTACH) {
        OutputDebugStringA("hi,this is dll_tls_callback1, DLL_PROCESS_ATTACH\n"); //crt���ܻ�û��ʼ�������ܵ���printf
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
����ʹ��crt���tls callback���ƣ����tlssup.cpp($(VSInstallDir)VC\crt\src\vcruntime\tlssup.cpp)
tlsup.cpp�ж����ˣ�
_CRTALLOC(".CRT$XLA") PIMAGE_TLS_CALLBACK __xl_a = 0;
_CRTALLOC(".CRT$XLZ") PIMAGE_TLS_CALLBACK __xl_z = 0;

���ڱ��������ƣ�".CRT$XLB"~".CRT$XLY"�ڵĵ�ַ����˳��ֲ���".CRT$XLA"��".CRT$XLZ"֮�䣬�γ�һ�������ĵ�ַ�ռ䣬���Ե���һ������
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