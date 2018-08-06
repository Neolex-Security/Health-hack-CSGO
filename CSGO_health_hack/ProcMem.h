#ifndef PROCMEM_H
#define PROCMEM_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <sstream>

class ProcMem{
protected:


	HANDLE hProcess;
	DWORD dwPID, dwProtection, dwCaveAddress;


	BOOL bPOn, bIOn, bProt;

public:


	ProcMem();
	~ProcMem();
	int chSizeOfArray(char *chArray);
	int iSizeOfArray(int *iArray);
	bool iFind(int *iAry, int iVal);
	int GetPid();
#pragma region TEMPLATE MEMORY FUNCTIONS





	template <class cData>
	cData Read(DWORD dwAddress)
	{
		cData cRead;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL);
		return cRead;
	}


	template <class cData>
	cData Read(DWORD dwAddress, char *Offset, BOOL Type)
	{

		int iSize = iSizeOfArray(Offset) - 1;
		dwAddress = Read<DWORD>(dwAddress);


		for (int i = 0; i < iSize; i++)
			dwAddress = Read<DWORD>(dwAddress + Offset[i]);

		if (!Type)
			return dwAddress + Offset[iSize];
		else
			return Read<cData>(dwAddress + Offset[iSize]);
	}



	template <class cData>
	void Read(DWORD dwAddress, char *Offset, cData Value)
	{
		Read<cData>(Read<cData>(dwAddress, Offset, false), Value);
	}


	template<typename T>
	void WriteMemory(DWORD dwAddress, T value)
	{
		WriteProcessMemory(hProcess,(LPVOID) dwAddress, &value, sizeof(T), NULL);
	}

	template<typename T>
	DWORD ProtectMemory(DWORD dwAddress, DWORD prot)
	{
		DWORD oldProt;
		VirtualProtectEx(hProcess,(LPVOID) dwAddress, sizeof(T), prot, &oldProt);
		return oldProt;
	}

	template<typename T>
	void WriteMemorySafe(DWORD dwAddress, T value)
	{
		DWORD oldProtect = ProtectMemory<DWORD>(dwAddress, PAGE_READWRITE);
		WriteMemory(dwAddress, value);
		ProtectMemory<DWORD>(dwAddress, oldProtect);
	}

	virtual void Process(std::wstring ProcessName);
	virtual DWORD Module(std::wstring ModuleName);

#pragma endregion

};
#endif

