// CSGO_health_hack.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
#include "ProcMem.h" // Memory reading
#include <iostream>
#include <chrono>
#include <thread>


using namespace std;
int main(void)
{
	ProcMem Mem; // Shortcut
	Mem.Process(L"csgo.exe"); 
	
	cout << "Counter Strike go PID : " << Mem.GetPid() << endl;

	DWORD ServerDLL = Mem.Module(L"server.dll"); //Module we are reading memory from
	

	/*
		base 0x26a30000
		pointer 1 = 0x2736e694  offset 1  '0x93e694'
		pointer 2 = 0x273a45c8 offset 2  '0x9745c8'
	*/

	const DWORD playerBase = 0x93e694;
	const DWORD healthOffset = 0x21c;


	while (true) {

		DWORD LocalPlayer = Mem.Read<DWORD>(ServerDLL + playerBase);
		cout << "Player health at 0x" << hex << LocalPlayer + healthOffset << endl;
		
		Mem.WriteMemorySafe(LocalPlayer + healthOffset, 1337);
		int health = Mem.Read<int>(LocalPlayer + healthOffset);
		cout << "Player's health : " << health << "." << endl;

		this_thread::sleep_for(chrono::milliseconds(600));
	}
	system("pause");
	return 0;
}
