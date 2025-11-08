#include <plugin.h>
#include <string>

using namespace plugin;

struct Main
{
	// Ugly but works
	int   const HEALTH_MAX = 200;			// NOTE: CPed.m_fMaxHealth seems to eventually glitch and get a random value after changing the ped's health.
	int   const DELAY = 15 * 1000;
	float const INCREMENT_HEALTH = 2.0f;
	float healthBefore = 0;
	uint64_t timeBefore = 0;
	uint64_t timeSinceDamage = 0;

    Main()
    {
        // register event callbacks
        Events::gameProcessEvent += [] { gInstance.OnGameProcess(); };
    }

    void OnGameProcess()
    {
		CPed* ped = FindPlayerPed(0);
		if (!ped)
			return;
		
		// 1. Make sure the player is alive
		// 2. there is a delay before healing (since function execs every frame)
		// 3. that the player doesn't have full health.
		if (ped->m_nDeathState || CTimer::m_snTimeInMilliseconds - timeBefore < 2.5 * 1000 || ped->m_fHealth >= HEALTH_MAX)
			return;

		// Restart if the player has been damaged again.
		if (ped->m_fHealth < healthBefore) {
			timeSinceDamage = CTimer::m_snTimeInMilliseconds;
			healthBefore = ped->m_fHealth;
			return;
		}

		// Wait for the delay before starting healing process.
		if (CTimer::m_snTimeInMilliseconds - timeSinceDamage < DELAY)
			return;

		// No returns, execute the healing.
		timeBefore = CTimer::m_snTimeInMilliseconds;

		if (ped->m_fHealth + INCREMENT_HEALTH > HEALTH_MAX)
			ped->SetHealth(HEALTH_MAX);
		else
			ped->AddHealth(INCREMENT_HEALTH);

		healthBefore = ped->m_fHealth;
    }
} gInstance;
