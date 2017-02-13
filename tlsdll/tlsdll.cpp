// tlsdll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

//�������и�����������PE�ļ���Ҫ����TLSĿ¼   
#pragma comment(linker, "/INCLUDE:__tls_used")   
/*����PIMAGE_TLS_CALLBACK()������ԭ�ͣ����е�һ���͵����������������ڶ��������������������������*/
/*һ�����ĸ�ѡ��DLL_PROCESS_ATTACH��DLL_THREAD_ATTACH��DLL_THREAD_DETACH��DLL_PROCESS_DETACH�����΢�����ġ�Microsoft Portable Executable and Common Object File Format Specification v8��*/
void NTAPI dll_tls_callback1(PVOID h, DWORD reason, PVOID pv) {
    if (reason == DLL_PROCESS_ATTACH) {
        printf("hi,this is %s, DLL_PROCESS_ATTACH, tid(%d)\n", __FUNCTION__, GetCurrentThreadId());
    } else if (reason == DLL_THREAD_ATTACH) {
        printf("hi,this is %s, DLL_THREAD_ATTACH, tid(%d)\n", __FUNCTION__, GetCurrentThreadId());;
    }
    return;
}

void NTAPI dll_tls_callback2(PVOID h, DWORD reason, PVOID pv) {
    if (reason == DLL_PROCESS_ATTACH) {
        printf("hi,this is %s, DLL_PROCESS_ATTACH, tid(%d)\n", __FUNCTION__, GetCurrentThreadId());
    } else if (reason == DLL_THREAD_ATTACH) {
        printf("hi,this is %s, DLL_THREAD_ATTACH, tid(%d)\n", __FUNCTION__, GetCurrentThreadId());;
    }
    return;;
}

/*
����ʹ��crt���tls callback���ƣ����tlsup.cpp($(VSInstallDir)VC\crt\src\vcruntime\tlsup.cpp)
tlsup.cpp�ж����ˣ�
_CRTALLOC(".CRT$XLA") PIMAGE_TLS_CALLBACK __xl_a = 0;
_CRTALLOC(".CRT$XLZ") PIMAGE_TLS_CALLBACK __xl_z = 0;

���ڱ��������ƣ�".CRT$XLB"~".CRT$XLY"�ڵĵ�ַ����˳��ֲ���".CRT$XLA"��".CRT$XLZ"֮�䣬�γ�һ�������ĵ�ַ�ռ䣬���Ե���һ������
*/
#pragma data_seg(".CRT$XLB")
PIMAGE_TLS_CALLBACK p_tls_callback[] = { dll_tls_callback1, dll_tls_callback2, 0 };
#pragma data_seg()

DWORD WINAPI ThreadProc(
    _In_ LPVOID lpParameter
) {
    printf("hi,this is %s, tid(%d)\n", __FUNCTION__, GetCurrentThreadId());
    return 0;
}

void test_thread() {
    HANDLE thread = CreateThread(NULL, 0, ThreadProc, p_tls_callback, 0, NULL);
    WaitForSingleObject(thread, INFINITE);
}