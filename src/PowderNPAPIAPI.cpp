/*****************************************************************************
* Copyright (c) 2014-2015 Branza Victor-Alexandru <branza.alex[at]gmail.com>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
*****************************************************************************/

#include <string.h>
#include <sstream>
#include <windows.h>

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "PowderNPAPIAPI.h"

using namespace std;

#pragma warning( disable : 4800 ) // hide warning about setting bool
#define BUFSIZE 512
#define BUFHEAD 16
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

string keepData = "";

string processLink[100] = { "" };
string processSeeds[100] = { "" };
string processFile[100] = { "" };
string processPath[100] = { "" };
string processDown[100] = { "" };
string processUp[100] = { "" };
string processPeerQueue[100] = { "" };
string processIdle[100] = { "" };
PROCESS_INFORMATION processInfo[100] = { 0 };


// kills a process (needed for killStream)
bool killProcess(const int pid) {
	STARTUPINFOA si = {0};
	PROCESS_INFORMATION pi = { 0 };

	std::string stringPid = std::to_string(pid);
		
	// argument to kill a specific peerflix instance
	string cmdArgs = "/C taskkill /f /pid ";
	cmdArgs.append(stringPid);
	cmdArgs.append(" /t");

	// Get windows directory
	TCHAR  infoBuf[32767];
	GetWindowsDirectory(infoBuf, 32767); 
	wstring toWstring(&infoBuf[0]);
	string cmdProg(toWstring.begin(), toWstring.end());

	// Direct path to cmd.exe
	cmdProg.append("\\system32\\cmd.exe");

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof si; //Only compulsory field


	if(CreateProcessA((LPCSTR)cmdProg.c_str(), (LPSTR)cmdArgs.c_str(), 
		NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,
		NULL,&si,&pi))
	{ 
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		return true;
	} else {
		return false;
	}
}

// Create a child process that uses the previously created pipes
//  for STDIN and STDOUT.
PROCESS_INFORMATION CreateChildProcess(const std::string& cmdmsg){
    // Set the text I want to run

	// Get windows directory
	TCHAR  infoBuf[32767];
	GetWindowsDirectory(infoBuf, 32767); 
	wstring toWstring(&infoBuf[0]);
	string szCmdline(toWstring.begin(), toWstring.end());

	// Call peerflix through node.exe
	szCmdline.append("\\system32\\cmd.exe /S /C \"\"%AppData%\\jaruba\\Powder NPAPI Plugin\\0.0.2\\deps\\node.exe\" \"%AppData%\\jaruba\\Powder NPAPI Plugin\\0.0.2\\deps\\peerflix\\app.js\" -r magnet:?xt=urn:btih:");
	szCmdline.append(cmdmsg);
	szCmdline.append("\"");

    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFOA siStartInfo;
    bool bSuccess = FALSE; 

    // Set up members of the PROCESS_INFORMATION structure. 
    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags = STARTF_USESTDHANDLES;

    // Create the child process. 
    bSuccess = CreateProcessA(NULL, 
        (LPSTR)szCmdline.c_str(),     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        CREATE_NO_WINDOW,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    CloseHandle(g_hChildStd_OUT_Wr);

    if ( ! bSuccess ) {
		PROCESS_INFORMATION emptyError = { 0 };
		return emptyError;
    }
    return piProcInfo;
}

// function to kill all peerflix processes (needed for killAll)
PROCESS_INFORMATION KillChildProcesses(){
    // Set the text I want to run

	// Get windows directory
	TCHAR  infoBuf[32767];
	GetWindowsDirectory(infoBuf, 32767); 
	wstring toWstring(&infoBuf[0]);
	string szCmdline(toWstring.begin(), toWstring.end());

	// Call taskkill to end all peerflix instances
	szCmdline.append("\\system32\\cmd.exe /C taskkill /f /fi \"Windowtitle eq peerflix\" /t");

    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFOA siStartInfo;
    bool bSuccess = FALSE; 

    // Set up members of the PROCESS_INFORMATION structure. 
    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags = STARTF_USESTDHANDLES;

    // Create the child process. 
    bSuccess = CreateProcessA(NULL, 
        (LPSTR)szCmdline.c_str(),     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        CREATE_NO_WINDOW,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
	CloseHandle(g_hChildStd_OUT_Wr);
    // If an error occurs, exit the application. 
    if ( ! bSuccess ) {
		PROCESS_INFORMATION emptyError = { 0 };
		return emptyError;
    }
    return piProcInfo;
}

// wait for command to finish running (needed for killAll)
void WaitForCommand(PROCESS_INFORMATION piProcInfo) {
    DWORD dwRead;
    CHAR chBuf[BUFSIZE];
    bool bSuccess = FALSE;
    for (;;) { 
        bSuccess=ReadFile( g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        if( ! bSuccess || dwRead == 0 ) break; 
    } 

}

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant PowderNPAPIAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant PowderNPAPIAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

	// kill all peerflix streams
	FB::variant PowderNPAPIAPI::killAll() {

		int i;
		for (i = 1; i < 101; i++) {
			processLink[i] = "";
			processSeeds[i] = "";
			processFile[i] = "";
			processPath[i] = "";
			processDown[i] = "";
			processUp[i] = "";
			processPeerQueue[i] = "";
			processIdle[i] = "";
		}


		// start peerflix process
		SECURITY_ATTRIBUTES sa; 

		// Set the bInheritHandle flag so pipe handles are inherited. 
		sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
		sa.bInheritHandle = TRUE; 
		sa.lpSecurityDescriptor = NULL; 

		// Create a pipe for the child process's STDOUT. 
		if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &sa, 0) ) {
			return "Error: Create Pipe";
		}
		// Ensure the read handle to the pipe for STDOUT is not inherited
		if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) ){
			return "Error: Set Handle";
		}

		PROCESS_INFORMATION procInfo = KillChildProcesses();
		WaitForCommand(procInfo);
		return true;

	}

	// kill a stream by stream index (returned by getStream)
	FB::variant PowderNPAPIAPI::killStream(int pIndex) {

		unsigned int tempPid = processInfo[pIndex].dwProcessId;

		if (tempPid == 0) {
			return false;
		} else {
			return killProcess(tempPid);
		}

	}

	// pipe the raw data from peerflix to the browser by stream index (returned by getStream)
	FB::variant PowderNPAPIAPI::readStream(int pIndex) {

//		string sData = ReadFromPipe(processInfo[pIndex]);

	DWORD dwRead;
	CHAR chBuf[BUFSIZE];
	DWORD avail;
	bool bSuccess = FALSE;
	bool tSuccess = FALSE;
	bool parseIt = FALSE;
	std::string out = "";
	tSuccess = PeekNamedPipe( g_hChildStd_OUT_Rd, NULL, 0, NULL, &avail, NULL );
	if (tSuccess && avail >= BUFSIZE) {
		while (avail >= BUFSIZE) {
			bSuccess=ReadFile( g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
			if( ! bSuccess || dwRead == 0 ) break; 
			std::string s(chBuf, dwRead);
			out += s;
			avail = avail - BUFSIZE;
		}
		parseIt = TRUE;
	} else if (tSuccess && avail >= BUFHEAD) {
		DWORD hdRead;
		CHAR hdBuf[BUFHEAD];
		tSuccess = PeekNamedPipe( g_hChildStd_OUT_Rd, hdBuf, BUFHEAD, &hdRead, &avail, NULL );
		if (tSuccess) {
			std::string hds(hdBuf, hdRead);
			bool doRead = FALSE;
			if (doRead) {
				while (avail >= BUFHEAD) {
					bSuccess=ReadFile( g_hChildStd_OUT_Rd, hdBuf, BUFHEAD, &dwRead, NULL);
					if( ! bSuccess || dwRead == 0 ) break; 
					std::string s(hdBuf, dwRead);
					out += s;
					avail = avail - BUFHEAD;
				}
				parseIt = TRUE;
			} else return false;
		} else return false;
	} else return false;
	if (parseIt) {
		if (out.find("[H[2J") != std::string::npos) out.substr(5);
//		if (out.find('[') != std::string::npos && out.find(']') != std::string::npos) doRead = TRUE;
		stringstream ss(out);
		string to;

		int ind = 0;

		while(std::getline(ss,to,'\n')){
			++ind;
			bool sData = false;
			if (to.find('[') != std::string::npos && to.find(']') != std::string::npos) {
				sData = true;
			} else {
				if (ind ==1) {
					sData = true;
					keepData.append(to);
					to = keepData;
				} else {
					keepData = to;
				}
			}
			if (sData) {
				bool doSend = FALSE;
				if (to.find("[\"streams\":") != std::string::npos && to != processSeeds[pIndex]) {
					processSeeds[pIndex] = to;
					doSend = TRUE;
				} else if (to.find("[\"link\":") != std::string::npos && to != processLink[pIndex]) {
					processLink[pIndex] = to;
					doSend = TRUE;
				} else if (to.find("[\"filename\":") != std::string::npos && to != processFile[pIndex]) {
					processFile[pIndex] = to;
					doSend = TRUE;
				} else if (to.find("[\"path\":") != std::string::npos && to != processPath[pIndex]) {
					processPath[pIndex] = to;
					doSend = TRUE;
				} else if (to.find("[\"downloaded\":") != std::string::npos && to != processDown[pIndex]) {
					processDown[pIndex] = to;
					doSend = TRUE;
				} else if (to.find("[\"uploaded\":") != std::string::npos && to != processUp[pIndex]) {
					processUp[pIndex] = to;
					doSend = TRUE;
				} else if (to.find("[\"runtime\":") != std::string::npos && to != processIdle[pIndex]) {
					processIdle[pIndex] = to;
					doSend = TRUE;
				} else if (to.find("[\"peerqueue\":") != std::string::npos && to != processPeerQueue[pIndex]) {
					processPeerQueue[pIndex] = to;
					doSend = TRUE;
				}
				if (doSend) {
					replace( to.begin(), to.end(), '[', '{');
					replace( to.begin(), to.end(), ']', '}');
					fire_torrentData(pIndex,to); // send data to JS Api through torrentData event
				}
			}
//					cout << to <<endl;
		}
		return true;
	} else return false;

//		return sData;

	}

	// accepts magnet link info hash and returns a stream index
	FB::variant PowderNPAPIAPI::getStream(const std::string& cmdmsg) {

		// check that infohash contains no spaces (for security reasons)
		std::string search = " ";
		if (cmdmsg.find(search) == std::string::npos) {
			// find an empty place in the multidimensional array to store the process data
			int i;
			for (i = 1; i < 101; i++) if (processLink[i] == "") break;

			processIdle[i] = "0";

			// start peerflix process
			SECURITY_ATTRIBUTES sa; 

			// Set the bInheritHandle flag so pipe handles are inherited. 
			sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
			sa.bInheritHandle = TRUE; 
			sa.lpSecurityDescriptor = NULL; 

			// Create a pipe for the child process's STDOUT. 
			if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &sa, 0) ) {
				return "Error: Create Pipe";
			}
			// Ensure the read handle to the pipe for STDOUT is not inherited
			if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) ){
				return "Error: Set Handle";
			}

			PROCESS_INFORMATION procInfo = CreateChildProcess(cmdmsg);
			processInfo[i] = procInfo;

			// return torrent index
			return i;

		} else {
			// error, the infohash contained space
			return false;
		}
	}


///////////////////////////////////////////////////////////////////////////////
/// @fn PowderNPAPIPtr PowderNPAPIAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
PowderNPAPIPtr PowderNPAPIAPI::getPlugin()
{
    PowderNPAPIPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string PowderNPAPIAPI::get_testString()
{
    return m_testString;
}

void PowderNPAPIAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string PowderNPAPIAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void PowderNPAPIAPI::testEvent()
{
    fire_test();
}