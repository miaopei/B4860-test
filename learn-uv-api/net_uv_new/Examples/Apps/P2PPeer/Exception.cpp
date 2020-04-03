#include "Exception.h"

#if _WIN32
#include "windows.h"
#include "DbgHelp.h"
#pragma comment(lib, "DbgHelp.lib")
#endif



std::string Exception::DumpFileName = TEXT("test.dmp");



// ����dump�ļ�
void CreateDempFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException)
{
	HANDLE hDumpFile = CreateFileA(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//dump��Ϣ
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	// д��dump�ļ�����
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}

// ����Unhandled Exception�Ļص�����
LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
	CreateDempFile(Exception::DumpFileName.c_str(), pException);
	return EXCEPTION_EXECUTE_HANDLER;
}

void Exception::registerException(const char* dumpFileName)
{
	Exception::DumpFileName = dumpFileName;
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
}
