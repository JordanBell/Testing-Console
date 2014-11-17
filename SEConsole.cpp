#include "SEConsole.h"
#include "Coin.h"
#include "Player.h"
#include "Game.h"
#include "Wallet.h"
#include "LaunchData.h"

// HAX

/* Launch all coins at the player */
void Pull(vector<int> args)
{
	int suppression = args.empty() ? 0 : args.front();

	for (Throwable* t : g_throwables)
	{
		if (!t->moving) {
			t->LaunchTo(g_player->x, g_player->y, suppression);
		}
	}
}

/* Make all coins bounce in place */
void BounceUp(vector<int> args)
{
	for (Throwable* t : g_throwables)
		if (!t->moving)
			t->BounceUp();
}

/* Make all coins bounce in place */
void SetBouncy(vector<int> args)
{
	for (Throwable* t : g_throwables)
		if (!t->moving)
			t->SetBouncy(args.front());
}

/* Temporary calculation function for printing arbitrary calculations used in debugging. 
Override the contents of this function for your own calculations when appropriate. */
void Calc(vector<int> args)
{
	// Prints calculated activation levels
	LaunchData::ComputeActivationLevels(true);
}



/* Enables Abilty: Pull. Press 1 to use. */
void TogglePull(vector<int> args)
	{ KeyCode::ToggleBool1(); }

/* Home coins toward the player */
void HomeIn(vector<int> args)
{
	int distance = args.size() > 0 ? args.at(0) : 400;
	int speed    = args.size() > 1 ? args.at(1) : 10;

	list<Throwable*> closeThrowables = Throwable::ThrowablesAroundPlayer(distance);
	for (Throwable* t : closeThrowables)
	{
		if (!t->moving) {
			t->SetHoming(distance, speed);
		}
	}
}

/* Forces Abilty: Smash. Specifies radius. */
void Smash(vector<int> args)
	{ g_player->Smash(args.front()); }

/* Forces Ability: Smash Wave */
void SmashWave(vector<int> args)
	{ g_player->SmashWave(); }


/* Toggles coin magnetism */
void ToggleMagnetism(vector<int> args)
	{ g_player->SetMagnetic( !g_player->IsMagnetic() ); }



/* Turn off all sounds */
void Mute(vector<int> args)
	{ g_game->Mute(); }

/* Turn on all sounds */
void Unmute(vector<int> args)
	{ g_game->Unmute(); }



/* Add any number of coins to the wallet */
void AddCoins(vector<int> args)
{
	int numCoins = args.front();
	Wallet::IncCoinsBy(numCoins);
}

/* Add a ludicrous number of coins to the wallet */
void AddCoins_Large(vector<int> args)
	{ Wallet::IncCoinsBy(999999999); }

/* Doubles the player's coins */
void DoubleCoins(vector<int> args)
	{ Wallet::IncCoinsBy(Wallet::GetCoins()); }



/* Dispense Coins */
void Dispense(vector<int> args)
	{ g_machine->ForceDispense(args.front()); }

/* Force Launch Tier */
void ForceTier(vector<int> args)
	{ LaunchData::ForceTier(args.front()); }

/* Print Information about the Tier's Launch Info */
void PrintLaunchInfo(vector<int> args)
	{ LaunchData::PrintLaunchInfo(); }


/* Jordan's debug preset */
void j(vector<int> args)
{
	TogglePull(args);
	ToggleMagnetism(args);

	// Toggle Mute
	if (g_game->IsMuted()) 
		Unmute(args);
	else
		Mute(args);
}



SEConsole::SEConsole(void)
{
	// All recognised commands
	commands.push_back( 
		Command("pull", 
		"Pulling all coins toward the player.", 
		"Launches all coins to the player.", 
		Pull)
	);

	commands.push_back( 
		Command("bounce", 
		"Bouncing.", 
		"Bounces all throwables up into the air.", 
		BounceUp)
	);

	commands.push_back( 
		Command("set_bounce", 
		"Bouncing set.", 
		"Pass an int boolean to set the bounciness of all throwables.", 
		SetBouncy)
	);
	
	commands.push_back( 
		Command("toggle_pull", 
		"Pull toggled. When enabled, press 1 to pull all coins toward the player.", 
		"Toggles the pull ability. Activate by pressing hotkey: 1", 
		TogglePull)
	);

	commands.push_back( 
		Command("mag", 
		"Magnetism Toggled", 
		"Toggles player magnetism, making it easier to collect coins.",
		ToggleMagnetism)
	);

	commands.push_back( 
		Command("smash", 
		"Smash Activated.", 
		"Simulates the 'Smash' Ability.",
		Smash)
	);

	commands.push_back( 
		Command("smash_wave", 
		"Wave Smash activated.", 
		"Simulates the 'Smash Wave' Ability.",
		SmashWave)
	);

	commands.push_back( 
		Command("mute", 
		"Muted all sounds.", 
		"Mutes coin collection sound effects.",
		Mute)
	);

	commands.push_back( 
		Command("unmute", 
		"Restored all sounds.", 
		"Restores all coin collection sound effects.",
		Unmute)
	);

	commands.push_back( 
		Command("add_coins", 
		"Coins added.", 
		"Adds a specified number of coins to the player's wallet.",
		AddCoins)
	);

	commands.push_back( 
		Command("max", 
		"Coins added. So many coins.", 
		"Adds 999999999 coins to the player's wallet.",
		AddCoins_Large)
	);

	commands.push_back( 
		Command("double_coins", 
		"Coins have been doubled", 
		"Doubles the player's coins in their wallet.",
		DoubleCoins)
	);

	commands.push_back( 
		Command("dispense", 
		"Dispensing...", 
		"Dispenses a specified number of coins directly into the game. May be unstable.",
		Dispense)
	);

	commands.push_back( 
		Command("home", 
		"Homing in on player.", 
		"Sets all collectables' homing property to true.",
		HomeIn)
	);

	commands.push_back( 
		Command("force_tier", 
		"Setting launch tier to manual value. WARNING: Collecting a coin will reset it.", 
		"Overrides the launch tier, which determines the possible launch patterns and styles unlocked. Also affects the type of coins launched.",
		ForceTier)
	);

	commands.push_back( 
		Command("print_launch_info", 
		"", 
		"Prints the kind of launch info available at the currently set tier.",
		PrintLaunchInfo)
	);

	commands.push_back( 
		Command("calc", 
		"Calculating: ", 
		"Performs a calculation. May change depending on the developer's choice of debug calculation.",
		Calc)
	);

	commands.push_back( 
		Command("j", 
		"Yes sir.", 
		"Jordan's preset of debug calls. Changes upon his mood. Originally [mag], [toggle_pull] and [mute].",
		j)
	);
}
