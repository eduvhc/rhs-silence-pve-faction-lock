//! Applies server-admin faction lock settings to campaign game modes.
modded class SCR_FactionManager
{
	//------------------------------------------------------------------------------------------------
	//! Allow the faction playability API to be used by RHSFL on the authority.
	void RHSFL_EnableRuntimeFactionChanges()
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

		GetGame().GetCallqueue().CallLater(RHSFL_ApplyFactionLock, 0, false);
	}

	//------------------------------------------------------------------------------------------------
	//! Load and apply the server's configured non-playable faction list.
	protected void RHSFL_ApplyFactionLock()
	{
		RHSFL_FactionLockSettings settings = new RHSFL_FactionLockSettings();
		settings.RHSFL_LoadOrCreate();
		if (!settings.RHSFL_HasDisabledFactions())
		{
			Print("[RHSFL] No factions configured for disabling");
			return;
		}

		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
		{
			Print("[RHSFL] Faction manager unavailable", LogLevel.ERROR);
			return;
		}

		factionManager.RHSFL_EnableRuntimeFactionChanges();
		int disabledCount = settings.RHSFL_DisableConfiguredFactions(factionManager);
		PrintFormat("[RHSFL] Disabled %1 faction(s)", disabledCount);
	}
}
