#include "RunMe_Console.h"

/* Print the word PRINT in all caps */
void PrintCaps(vector<int> args)
{
	print("PRINT\n");
}

/* Take a number, and print its cast boolean value. */
void PrintCaps(vector<int> args)
{
	bool b = (bool)args.front();
	char* boolStr = b? "True" : "False";
	print("%s\n", boolStr);
}



RunMe_Console::RunMe_Console(void)
{
	// All recognised commands
	m_commands.push_back( 
		Command("print_caps", 
		"Printing PRINT... ", 
		"Print's the 'PRINT' string.", 
		PrintCaps)
	);
	
	m_commands.push_back( 
		Command("printBool", 
		"As a boolean, that number is... ", 
		"Takes a number, and gives its cast boolean value.", 
		PrintBool)
	);
	
	m_commands.push_back( 
		Command("add", 
		"", 
		"Adds a list of integer arguments together.", 
		Add)
	);
}
