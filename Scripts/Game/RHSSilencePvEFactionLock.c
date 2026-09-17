// Server-side settings for RHS Conflict-compatible scenarios.
// The settings file is created at $profile:RHSSilencePvEFactionLock/settings.json.
class RHSFactionLockSettings
{
	static const string DIRECTORY = "$profile:RHSSilencePvEFactionLock";
	static const string FILE_PATH = DIRECTORY + "/settings.json";

	ref array<string> m_aDisabledFactions = {"RHS_AFRF", "FIA"};

	void LoadOrCreate()
	{
		if (!FileIO.FileExists(FILE_PATH))
		{
			if (!FileIO.MakeDirectory(DIRECTORY))
			{
				Print("[RHS Faction Lock] Cannot create settings directory; defaults are being used", LogLevel.WARNING);
				return;
			}

			Save();
			return;
		}

		auto context = new JsonLoadContext();
		if (!context.LoadFromFile(FILE_PATH))
		{
			Print("[RHS Faction Lock] Invalid settings.json; defaults are being used", LogLevel.WARNING);
			return;
		}

		context.ReadValue("disabledFactions", m_aDisabledFactions);
	}

	protected void Save()
	{
		auto context = new PrettyJsonSaveContext();
		context.WriteValue("disabledFactions", m_aDisabledFactions);
		context.SaveToFile(FILE_PATH);
	}
}

// Conflict scenarios can disable runtime faction changes in their world configuration.
// The addon enables them only on the server, immediately before applying its JSON settings.
modded class SCR_FactionManager
{
	void RHSFactionLock_EnableRuntimeFactionChanges()
	{
		m_bCanChangeFactionsPlayable = true;
	}
}

modded class SCR_GameModeCampaign
{
	override void OnGameStart()
	{
		super.OnGameStart();

		if (!Replication.IsServer())
			return;

		GetGame().GetCallqueue().CallLater(ApplyRHSFactionLock, 0, false);
	}

	protected void ApplyRHSFactionLock()
	{
		auto settings = new RHSFactionLockSettings();
		settings.LoadOrCreate();
		if (settings.m_aDisabledFactions.IsEmpty())
		{
			Print("[RHS Faction Lock] No factions configured for disabling");
			return;
		}

		SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		if (!factionManager)
		{
			Print("[RHS Faction Lock] Faction manager unavailable", LogLevel.ERROR);
			return;
		}

		factionManager.RHSFactionLock_EnableRuntimeFactionChanges();
		int disabledCount = 0;
		foreach (string factionKey : settings.m_aDisabledFactions)
		{
			if (DisableFaction(factionManager, factionKey))
				disabledCount++;
		}

		PrintFormat("[RHS Faction Lock] Disabled %1 faction(s)", disabledCount);
	}

	protected bool DisableFaction(SCR_FactionManager factionManager, string factionKey)
	{
		if (factionKey.IsEmpty())
			return false;

		SCR_Faction faction = SCR_Faction.Cast(factionManager.GetFactionByKey(factionKey));
		if (!faction)
		{
			PrintFormat("[RHS Faction Lock] Unknown faction key: %1", factionKey, level: LogLevel.WARNING);
			return false;
		}

		faction.SetIsPlayable(false);
		return true;
	}
}
