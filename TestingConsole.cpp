#include "TestingConsole.h"
#include <cstdlib>
#include <iostream>

void TestingConsole::Open(void)  
{ 
	m_active = true;  
	printf("Console Active. Enter 'help' for a list of commands."); 
	SDL_EnableUNICODE( SDL_ENABLE );
}

void TestingConsole::Close(void) 
{ 
	m_active = false;  
	printf("\nConsole Inactive\n"); 
	SDL_EnableUNICODE( SDL_DISABLE );
}

void TestingConsole::Toggle(void) 
{ 
	m_active ? Close() : Open(); 
	if (m_active) NewLine();
}

void TestingConsole::KeyIn(SDL_keysym& keysym)
{
	if (keysym.sym == SDLK_BACKSPACE) // Backspace pops the last element of the string
	{ 
		if (m_line.size() > 0) {
            m_line.pop_back();
            printf("\b \b"); // Backspace
		}
	}
	else if (keysym.sym == SDLK_RETURN) // Enter enters the code into the console
	{ 
		printf("\n");
		Enter();
		NewLine();
	}
	else if (keysym.sym == SDLK_UP) // Traverse UP along the command memory
	{ 
		string commandStr = "";

		if (!commandMemory.empty())
		{
			if (memoryIterator != commandMemory.begin()) {
				memoryIterator--;
				commandStr = *memoryIterator;
			}
		}

		if (commandStr != "")
			OverrideLine(commandStr);
	}
	else if (keysym.sym == SDLK_DOWN) // Traverse UP along the command memory
	{ 
		string commandStr = "";

		if (!commandMemory.empty())
		{
			if (memoryIterator != commandMemory.end())
			{
				memoryIterator++;
				if (memoryIterator != commandMemory.end())
					commandStr = *memoryIterator;
			}
		}
		
		OverrideLine(commandStr);
	}
	else 
	{ // Otherwise, just input the character
		if (ValidationInput(keysym))
			printf("%c", (char)(keysym.unicode));
	}
}

void TestingConsole::OverrideLine(string _line)
{
	// Replace the output on the command line with the new string
	int strSize = m_line.size();
	for (int i = 0; i < strSize; i++)
		printf("\b \b");

	// Set the new line
	m_line = _line;
	// Print it
	printf("%s", m_line.c_str());
}


/* Enter the current m_line into the console, invoking any corresponding functions. */
void TestingConsole::Enter(void)
{ 
	// Add this line to the list of entered command strings
	commandMemory.remove(m_line);
	commandMemory.push_back(m_line);
	memoryIterator = commandMemory.end();

	// Separate the activation command from its arguments, if any
	pair<string, string> codeArgumentsPair = SplitCommandCode(m_line);
	string activationCode = codeArgumentsPair.first;
	vector<int> arguments = ExtractArguments(codeArgumentsPair.second);

	if (m_line == "help") {
		CommandHelp();
		return;
	}

	for (Command possibleCommand : commands)
	{
		if (activationCode == possibleCommand.code)
		{
			try
			{
				// Execute the function
				possibleCommand.func(arguments);
				// Output the command's message, ie "Pull activated."
				printf("%s", possibleCommand.message.c_str());
			}
			catch (exception e)
			{
				// If, for some reason, the called function does not operate correctly and throws an exception, fail and notify the user. Don't crash the game.
				printf("Error. Called function threw an exception: %s", e.what());
			}

			// Reset the m_line
			m_line.clear();
			// No point to continue searching
			return;
		}
	}

	// If no commands are recognised by now
	printf("Command [%s] not recognised.", activationCode.c_str());
	m_line.clear();
}

void TestingConsole::CommandHelp(void)
{
	printf("Available commands:\n\n");

	for (Command c : commands)
	{
		printf("[%s] = %s\n", 
				c.code.c_str(), 
				c.help.c_str());
	}

	NewLine();
}


pair<string, string> TestingConsole::SplitCommandCode(string line)
{
	pair<string, string> codeArgumentsPair; // Return
	string head = "";
	string tail = line;

	for (int i = 0; i < line.size(); i++)
	{
		char c = line.at(i);
		tail.erase(0, 1);

		if (c == ' ') {
			// End of command code, beginning of arguments
			codeArgumentsPair.first  = head;
			codeArgumentsPair.second = tail;
			return codeArgumentsPair;
		}
		else
			head.push_back(c);

	}

	codeArgumentsPair.first = line;
	codeArgumentsPair.second = "";
	return codeArgumentsPair;
}

vector<int> TestingConsole::ExtractArguments(string argumentsString)
{
	vector<int> r_args;

	string head = "";
	string tail = argumentsString;

	for (int i = 0; i < argumentsString.size(); i++)
	{
		char c = argumentsString.at(i);
		tail.erase(0, 1);

		if (c == ' ') {
			r_args.push_back(atoi(head.c_str()));
			head = "";
		}
		else
		{
			head.push_back(c);
		}

	}

	// Add the last argument
	r_args.push_back(atoi(head.c_str()));

	return r_args;
}

bool TestingConsole::ValidationInput(SDL_keysym keysym)
{
	bool isMisc = ((keysym.unicode == (Uint16)' ') || (keysym.unicode == (Uint16)'_'));
	bool isNumber = ((keysym.unicode >= (Uint16)'0') && (keysym.unicode <= (Uint16)'9'));
	bool isLowercase = ((keysym.unicode >= (Uint16)'a') && (keysym.unicode <= (Uint16)'z'));
	bool isUppercase = ((keysym.unicode >= (Uint16)'A') && (keysym.unicode <= (Uint16)'Z'));

	// Add the character if it 
    if (isMisc || isNumber || isLowercase || isUppercase)
    {
        //Append the character
        m_line += (char)keysym.unicode;
    }

	return (isMisc || isNumber || isLowercase || isUppercase);
}