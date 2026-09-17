//! Applies server-admin faction availability settings to campaign game modes.
modded class SCR_FactionManager
{
	//------------------------------------------------------------------------------------------------
	//! Allow the faction playability API to be used by Faction Availability Lock on the authority.
	void FAL_EnableRuntimeFactionChanges()
	{
		m_bCanChangeFactionsPlayable = true;
	}
}

modded class SCR_GameModeCampaign
{
	//------------------------------------------------------------------------------------------------
	override void OnGameStart()
	{
		super.OnGameStart();

		if (!Replication.IsServer())
			return;

		GetGame().GetCallqueue().CallLater(FAL_ApplyFactionLock, 0, false);
	}

	//------------------------------------------------------------------------------------------------
	//! Load and apply the server's configured non-playable faction list.
	protected void FAL_ApplyFactionLock()
	{
		FAL_FactionLockSettings settings = new FAL_FactionLockSettings();
		settings.FAL_LoadOrCreate();
		if (!settings.FAL_HasDisabledFactions())
		{
			Print("[FactionAvailabilityLock] No factions configured for disabling");
			return;
		}

		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
		{
			Print("[FactionAvailabilityLock] Faction manager unavailable", LogLevel.ERROR);
			return;
		}

		factionManager.FAL_EnableRuntimeFactionChanges();
		int disabledCount = settings.FAL_DisableConfiguredFactions(factionManager);
		PrintFormat("[FactionAvailabilityLock] Disabled %1 faction(s)", disabledCount);
	}
}
