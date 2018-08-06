#include "stdafx.h"
#include "ProcMem.h"

using namespace std;

#pragma region Misc Functions

ProcMem::ProcMem(){

}

ProcMem::~ProcMem(){
	CloseHandle(hProcess);
}

int ProcMem::GetPid()
{
	return (int)dwPID;
}

int ProcMem::chSizeOfArray(char *chArray){


	for (int iLength = 1; iLength < MAX_PATH; iLength++)
		if (chArray[iLength] == '*')
			return iLength;

	cout << "\nLENGTH: Failed To Read Length Of Array\n";
	return 0;
}


int ProcMem::iSizeOfArray(int *iArray){


	for (int iLength = 1; iLength < MAX_PATH; iLength++)
		if (iArray[iLength] == '*')
			return iLength;

	cout << "\nLENGTH: Failed To Read Length Of Array\n";
	return 0;
}


bool ProcMem::iFind(int *iAry, int iVal){

	for (int i = 0; i < 64; i++)
		if (iVal == iAry[i] && iVal != 0)
			return true;

	return false;
}

#pragma endregion



#pragma region Memory Functions


void ProcMem::Process(wstring ProcessName){


	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry);


	do {
		wstring binaryPath = ProcEntry.szExeFile;
		if (binaryPath.find(ProcessName) != wstring::npos)
		{
			dwPID = ProcEntry.th32ProcessID;
			CloseHandle(hPID);
			
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
			return;
		}
	}while (Process32Next(hPID, &ProcEntry));

	cout << "\nPROCESS: Process Not Found\n";
	system("pause");
	exit(0);
}




DWORD ProcMem::Module(std::wstring ModuleName) {


	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);


	do {
		std::wstring modulePath = mEntry.szModule;
		if (modulePath.find(ModuleName) != std::wstring::npos)
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	} while (Module32Next(hModule, &mEntry));

	cout << "\nMODULE: Process Platform Invalid\n";
	return 0;
}

#pragma endregion