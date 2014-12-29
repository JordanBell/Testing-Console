//
//  TestingConsole.cpp
//  Testing-Console
//
//  Created by Jordan Bell on 15/10/2014.
//  Copyright (c) 2014 Jordan Bell. All rights reserved.
//

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
	SDLKey key = keysym.sym;
	if (key == SDLK_BACKSPACE) // Backspace 
	{ 
		// Pop the last element of the string in the line
		if (m_line.size() > 0) {
            m_line.pop_back();
            printf("\b \b"); // Backspace
		}
	}
	else if (key == SDLK_RETURN) // Enter/Return
	{ 
		// Enter the code into the console
		printf("\n");
		Enter();
		NewLine();
	}
	else if (key == SDLK_UP) // Traverse UP along the command memory
	{ 
		string commandStr = "";

		if (!m_commandMemory.empty())
		{
			if (m_memoryIterator != m_commandMemory.begin()) {
				m_memoryIterator--;
				commandStr = *m_memoryIterator;
			}
		}

		if (commandStr != "")
			OverrideLine(commandStr);
	}
	else if (key == SDLK_DOWN) // Traverse DOWN along the command memory
	{ 
		string commandStr = "";

		if (!m_commandMemory.empty())
		{
			if (m_memoryIterator != m_commandMemory.end())
			{
				m_memoryIterator++;
				if (m_memoryIterator != m_commandMemory.end())
					commandStr = *m_memoryIterator;
			}
		}
		
		OverrideLine(commandStr);
	}
	else 
	{ 
		// Otherwise, just input the character
		char keyChar = keysym.unicode;
		if (ValidationInput(keyChar))
			printf("%c", keyChar);
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

void TestingConsole::Enter(void)
{ 
	// Add this line to the list of entered command strings
	m_commandMemory.remove(m_line);
	m_commandMemory.push_back(m_line);
	m_memoryIterator = m_commandMemory.end();

	// Separate the activation command from its arguments, if any
	pair<string, string> codeArgumentsPair = SplitCommandCode(m_line);
	string activationCode = codeArgumentsPair.first;
	vector<int> arguments = ExtractArguments(codeArgumentsPair.second);

	if (m_line == "help") {
		CommandHelp();
		return;
	}

	// Search through all of the commands for a match
	for (Command possibleCommand : m_commands)
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

	for (Command c : m_commands)
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

bool TestingConsole::ValidationInput(char keyChar)
{
	// All valid character types
	bool isMisc = ((keyChar == ' ') || (keyChar == '_'));
	bool isNumber = ((keyChar >= '0') && (keyChar <= '9'));
	bool isLowercase = ((keyChar >= 'a') && (keyChar <= 'z'));
	bool isUppercase = ((keyChar >= 'A') && (keyChar <= 'Z'));

	// Add the character if it is any of the above
    if (isMisc || isNumber || isLowercase || isUppercase)
    {
        //Append the character
        m_line += (char)keyChar;
    }

	return (isMisc || isNumber || isLowercase || isUppercase);
}