#include "CGConsole.h"
#include "Coin.h"
#include "Player.h"
#include "Game.h"
#include "Wallet.h"
#include "LaunchData.h"
#include "ParticleSnow.h"
#include "Camera.h"

#define DIVIDES(a, b) (a%b == 0)

/* Temporary calculation function for printing arbitrary calculations used in debugging. 
Override the contents of this function for your own calculations when appropriate. */
void Calc(vector<int> args)
{
	// Nothing
}

/* Launch all coins at the player */
void Pull(vector<int> args)
{
	int suppression = args.empty() ? 0 : args.front();

	for (Throwable* t : g_throwables)
	{
		if (!t->IsAirborne()) {
			t->LaunchTo(g_player->x, g_player->y, suppression);
		}
	}
}

/* Make a small explosion of simple particles */
void ParticleExplosion(vector<int> args)
{
	int s_x, s_y; // Start coords
	s_x = screen->w/2 - TILE_SIZE/2;
	s_y = screen->h/2 - TILE_SIZE/2+2*TILE_SIZE;

	int e_x, e_y; // End coords

	list<Particle*> particles;
	int num = args.front();
	for (int i = 0; i < num; i++)
	{
		// Dirt flies somewhere around behind the player
		e_x = s_x + (rand() % (6*TILE_SIZE)) - 3*TILE_SIZE;
		e_y = s_y + (rand() % (4*TILE_SIZE)) - 2*TILE_SIZE;

		ParticleSimple* part = new ParticleSimple(s_x, s_y, e_x, e_y);

		particles.push_back(part);
		g_game->addGameObject(part);
	}

	for (Particle* p : particles)
		p->Launch(1);
}


/* Make all coins bounce in place */
void BounceUp(vector<int> args)
{
	for (Throwable* t : g_throwables)
		if (!t->IsAirborne())
			t->BounceUp();
}

/* Make all coins bounce in place */
void SetBouncy(vector<int> args)
{
	for (Throwable* t : g_throwables)
		if (!t->IsAirborne())
			t->SetBouncy(args.front());
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
		if (!t->IsAirborne()) {
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
	{ //g_player->SetMagnetic( !g_player->IsMagnetic() );
    }



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
	{ g_camera->GetRoomFocus()->GetMachine()->ForceDispense(args.front()); }

/* Print Information about the Tier's Launch Info */
void Talk(vector<int> args)
{ 
	g_player->Say("Hey.");
}


/* Jordan's debug preset */
void j(vector<int> args)
{
	//TogglePull(args);
	//ToggleMagnetism(args);

	//// Toggle Mute
	//if (g_game->IsMuted()) 
	//	Unmute(args);
	//else
	//	Mute(args);

	/*g_machine->ForceDispense(1000);
	Mute(args);*/
	
	// Focus the camera over the fire room's sinkhole.
	g_camera->DisableUpdate();
	s_renderingOffset_x = -screen->w;
	s_renderingOffset_y = screen->h+10*TILE_SIZE;
}


/* Set the rendering offset values */
void SetRenderOffset(vector<int> args)
{
	s_renderingOffset_x = args.at(0);
	s_renderingOffset_y = args.at(1);
}



CGConsole::CGConsole(void)
{
	// All recognised commands
	m_commands.push_back( 
		Command("pull", 
		"Pulling all coins toward the player.", 
		"Launches all coins to the player.", 
		Pull)
	);

	m_commands.push_back( 
		Command("bounce", 
		"Bouncing.", 
		"Bounces all throwables up into the air.", 
		BounceUp)
	);

	m_commands.push_back( 
		Command("set_bounce", 
		"Bouncing set.", 
		"Pass an int boolean to set the bounciness of all throwables.", 
		SetBouncy)
	);
	
	m_commands.push_back( 
		Command("toggle_pull", 
		"Pull toggled. When enabled, press 1 to pull all coins toward the player.", 
		"Toggles the pull ability. Activate by pressing hotkey: 1", 
		TogglePull)
	);

	m_commands.push_back( 
		Command("mag", 
		"Magnetism Toggled", 
		"Toggles player magnetism, making it easier to collect coins.",
		ToggleMagnetism)
	);

	m_commands.push_back( 
		Command("smash", 
		"Smash Activated.", 
		"Simulates the 'Smash' Ability.",
		Smash)
	);

	m_commands.push_back( 
		Command("smash_wave", 
		"Wave Smash activated.", 
		"Simulates the 'Smash Wave' Ability.",
		SmashWave)
	);

	m_commands.push_back( 
		Command("mute", 
		"Muted all sounds.", 
		"Mutes coin collection sound effects.",
		Mute)
	);

	m_commands.push_back( 
		Command("unmute", 
		"Restored all sounds.", 
		"Restores all coin collection sound effects.",
		Unmute)
	);

	m_commands.push_back( 
		Command("add_coins", 
		"Coins added.", 
		"Adds a specified number of coins to the player's wallet.",
		AddCoins)
	);

	m_commands.push_back( 
		Command("max", 
		"Coins added. So many coins.", 
		"Adds 999999999 coins to the player's wallet.",
		AddCoins_Large)
	);

	m_commands.push_back( 
		Command("double_coins", 
		"Coins have been doubled", 
		"Doubles the player's coins in their wallet.",
		DoubleCoins)
	);

	m_commands.push_back( 
		Command("dispense", 
		"Dispensing...", 
		"Dispenses a specified number of coins directly into the game. May be unstable.",
		Dispense)
	);

	m_commands.push_back( 
		Command("home", 
		"Homing in on player.", 
		"Sets all collectables' homing property to true.",
		HomeIn)
	);

	m_commands.push_back( 
		Command("calc", 
		"Calculating: ", 
		"Performs a calculation. May change depending on the developer's choice of debug calculation.",
		Calc)
	);

	m_commands.push_back( 
		Command("offset", 
		"Setting the offset ", 
		"Gives a manual value to the rendering offset values, x and y respectively.",
		SetRenderOffset)
	);

	m_commands.push_back( 
		Command("j", 
		"Yes sir.", 
		"Jordan's preset of debug calls. Changes upon his mood. Originally [mag], [toggle_pull] and [mute].",
		j)
	);

	m_commands.push_back( 
		Command("exp", 
		"Exploding particles", 
		"Adds a small, standard explosion of simple particles at (320,320).",
		ParticleExplosion)
	);

	m_commands.push_back( 
		Command("talk", 
		"", 
		"Tests speech bubbles by making the player talk",
		Talk)
	);
}
