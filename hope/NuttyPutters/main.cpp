// Externals

// Internals
#include "windowMgr.h"


// GLOBALS
windowMgr* winMgr; // Pointer to winMgr since it's a singleton




int main(int argc, char **argv)
{
	// Get singleton instance of windowMgr
	winMgr = windowMgr::getInstance();
	
	// Setup window
	winMgr->Init();

	// Run update loop
	winMgr->Update();

	// On window close
	winMgr->CleanUp();

	return 0;
}