#include <array>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "MyPhantomJS.h"

namespace MyPhantomJS {

//
// Execute a command and get the results. (Only standard output)
// It does not open the console.
//
std::string ExecCmd(const std::string cmd) {
	std::string strResult;
	HANDLE hPipeRead, hPipeWrite;

	SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES)};
	saAttr.bInheritHandle = TRUE; // Pipe handles are inherited by child process.
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe to get results from child's stdout.
	if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0)) {
		std::cout << "Cannot create pipe" << std::endl;
		return strResult;
	}

	STARTUPINFOW si = {sizeof(STARTUPINFOW)};
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = hPipeWrite;
	si.hStdError = hPipeWrite;
	si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing.
							  // Requires STARTF_USESHOWWINDOW in dwFlags.

	PROCESS_INFORMATION pi = {0};

	std::wstring wcmd(cmd.begin(), cmd.end());
	BOOL fSuccess = CreateProcessW(NULL, (LPWSTR)wcmd.c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
								   NULL, NULL, &si, &pi);
	if (!fSuccess) {
		CloseHandle(hPipeWrite);
		CloseHandle(hPipeRead);
		std::cout << "CreateProcessW error: " << GetLastError() << std::endl;
		return strResult;
	}

	bool bProcessEnded = false;
	for (; !bProcessEnded;) {
		// Give some timeslice (50 ms), so we won't waste 100% CPU.
		bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

		// Even if process exited - we continue reading, if
		// there is some data available over pipe.
		for (;;) {
			char buf[1024];
			DWORD dwRead = 0;
			DWORD dwAvail = 0;

			if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
				break;

			if (!dwAvail) // No data available, return
				break;

			if (!::ReadFile(hPipeRead, buf, min(sizeof(buf) - 1, dwAvail), &dwRead, NULL) ||
				!dwRead)
				// Error, the child process might ended
				break;

			buf[dwRead] = 0;
			strResult += buf;
		}
	} // for

	CloseHandle(hPipeWrite);
	CloseHandle(hPipeRead);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return strResult;
}

std::string urlToString(std::string url) {
	return ExecCmd(
		"phantomjs\\phantomjs.exe --cookies-file=phantomjs\\cookies.txt phantomjs\\getHtml.js \"" +
		url + "\"");
}

} // namespace MyPhantomJS
