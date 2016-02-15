// wrapper for dlldata.c

#ifdef _MERGE_PROXYSTUB // merge proxy stub DLL

#define REGISTER_PROXY_DLL //DllRegisterServer, etc.

#ifndef _WIN32_WCE
#define _WIN32_WINNT 0x0400	//for WinNT 4.0 or Win95 with DCOM
#else
#define WIN32
#endif
#define USE_STUBLESS_PROXY	//defined only with MIDL switch /Oicf

#ifndef _WIN32_WCE
#pragma comment(lib, "rpcns4.lib")
#endif
#pragma comment(lib, "rpcrt4.lib")

#define ENTRY_PREFIX	Prx

#include "dlldata.c"
#include "PoomWrapper_p.c"

#endif //_MERGE_PROXYSTUB
