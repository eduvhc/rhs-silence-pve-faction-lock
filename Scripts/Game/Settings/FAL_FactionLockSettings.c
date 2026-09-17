//! Server-admin settings for Faction Availability Lock.
class FAL_FactionLockSettings
{
	static const string DIRECTORY = "$profile:FactionAvailabilityLock";
	static const string FILE_PATH = "$profile:FactionAvailabilityLock/settings.json";

	protected ref array<string> m_aDisabledFactions = {};

	//------------------------------------------------------------------------------------------------
	//! Load settings from the server profile, or create a safe empty default file.
	void FAL_LoadOrCreate()
	{
		if (!FileIO.FileExists(FILE_PATH))
		{
			if (!FileIO.FileExists(DIRECTORY) && !FileIO.MakeDirectory(DIRECTORY))
			{
				Print("[FactionAvailabilityLock] Cannot create settings directory; using defaults", LogLevel.WARNING);
				return;
			}

			if (FAL_Save())
				Print("[FactionAvailabilityLock] Wrote default settings to " + FILE_PATH);

			return;
		}

		JsonLoadContext context = new JsonLoadContext();
		if (!context.LoadFromFile(FILE_PATH))
		{
			Print("[FactionAvailabilityLock] Invalid settings.json; using defaults", LogLevel.WARNING);
			return;
		}

		context.ReadValue("disabledFactions", m_aDisabledFactions);
		if (!m_aDisabledFactions)
			m_aDisabledFactions = {};

		Print("[FactionAvailabilityLock] Loaded settings from " + FILE_PATH);
	}

	//------------------------------------------------------------------------------------------------
	//! Return true when the settings contain at least one faction key.
	bool FAL_HasDisabledFactions()
	{
		return !m_aDisabledFactions.IsEmpty();
	}

	//------------------------------------------------------------------------------------------------
	//! Disable each configured faction that exists and is currently playable.
	//! \param factionManager The active scenario faction manager.
	//! \return Number of factions whose availability changed.
	int FAL_DisableConfiguredFactions(SCR_FactionManager factionManager)
	{
		int disabledCount = 0;
		foreach (string factionKey : m_aDisabledFactions)
		{
			if (FAL_DisableFaction(factionManager, factionKey))
				disabledCount++;
		}

		return disabledCount;
	}

	//------------------------------------------------------------------------------------------------
	//! Write the settings file.
	//! \return True when the file was written successfully.
	protected bool FAL_Save()
	{
		PrettyJsonSaveContext context = new PrettyJsonSaveContext();
		context.WriteValue("disabledFactions", m_aDisabledFactions);
		if (context.SaveToFile(FILE_PATH))
			return true;

		Print("[FactionAvailabilityLock] Failed to write " + FILE_PATH, LogLevel.WARNING);
		return false;
	}

	//------------------------------------------------------------------------------------------------
	//! Disable one configured faction.
	//! \param factionManager The active scenario faction manager.
	//! \param factionKey Faction key to disable.
	//! \return True when a playable faction was changed to non-playable.
	protected bool FAL_DisableFaction(SCR_FactionManager factionManager, string factionKey)
	{
		if (factionKey.IsEmpty())
			return false;

		SCR_Faction faction = SCR_Faction.Cast(factionManager.GetFactionByKey(factionKey));
		if (!faction)
		{
			PrintFormat("[FactionAvailabilityLock] Unknown faction key: %1", factionKey, level: LogLevel.WARNING);
			return false;
		}

		if (!faction.IsPlayable())
			return false;

		faction.SetIsPlayable(false);
		return true;
	}
}
