#pragma once
#include <string>
#include <list>
#include <functional>
#include <type_traits>
#include <vector>
#include "SDL.h"
using namespace std;

class TestingConsole
{
public:
	TestingConsole(void) : m_active(false), m_prompt("~: "), m_memoryIterator(m_commandMemory.begin()) {}
	~TestingConsole(void) {}
	
	// Activate or Deactivate the Testing Console for input
	void Toggle(void);

	// Return whether or not the Testing Console is currently active
	bool IsActive(void) { return m_active; }

	/* Enter the current m_line into the console, invoking any corresponding functions. */
	void Enter(void);

	// Enter a key into the Testing Console
	void KeyIn(SDL_keysym& keysym);

protected:
	/*
	A struct containing information pertaining to the various commands that can be recognised by the console.
	Object of this struct can be instantiated in subclasses of the Testing Console. Otherwise, no commands are recognised.
	*/
	struct Command
	{
		// Standard Constructor
		Command(string _code, string _message, void (*_func)(vector<int>))
			: code(_code), message(_message), help("No description."), func(_func) {}

		// Constructor, with a help message defined for the Help menu
		Command(string _code, string _message, string _help, void (*_func)(vector<int>))
			: code(_code), message(_message), help(_help), func(_func) {}

		string code;	// The string that invokes a console response
		string help;	// The description displayed on the help menu
		string message;	// The response that is printed to acknowledge its activation
		void (*func)(vector<int>);	// The function to be called when the matching string is enterred.
	};

	// The list of commands recognised by the testing console
	list<Command> m_commands;

private:
	string m_prompt; // The "prompt" displayed in the console before the user input
	list<string> m_commandMemory; // The list of previously enterred commands, cycled through using the UP and DOWN keys
	list<string>::iterator m_memoryIterator; // The iterator through the list of command memories
	string m_line; // The string currently enterred into the console
	bool m_active; // Defines whether or not this console will accept input

	// Return a pair of strings. The first being the activation code, the second being the arguments
	static pair<string, string> SplitCommandCode(string line); 

	// Returns a vector of arguments from a string of suppose arguments
	static vector<int> ExtractArguments(string argumentsString); 

	// Open the console, enabling SDL unicode input and prompting the user
	void Open(void);

	// Close the console, notifying the user of its closing and disabling SDL unicode
	void Close(void);

	/*
	* Add a character to the m_line, given that it is valid. Ignore, if not.
	* Take a keysym object containing relevant information for validation
	* Return a boolean indicating the validity of that key character
	*/
	bool ValidationInput(char keysym);

	// Print the Help menu - all of the currently held commands, and their corresponding descriptions, if provided.
	void CommandHelp(void);

	// Add a new line, clearing the current m_line value and starting again
	void NewLine(void) { m_line.clear(); printf("\n%s", m_prompt.c_str()); }

	// Override m_line's value with a given string, and delete its output in the console
	void OverrideLine(string _line);
};

