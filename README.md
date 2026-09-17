# RHS Silence PvE Faction Lock

Small server-side addon for Arma Reforger Conflict scenarios using RHS factions. It removes configured factions from the faction-selection and respawn menus.

The default configuration disables `RHS_AFRF` and `FIA`; it does not choose or enable a player faction.

## Requirements

- Arma Reforger
- RHS - Status Quo
- RHS - Content Pack 01
- RHS - Content Pack 02
- A scenario derived from `SCR_GameModeCampaign` (including compatible Silence PvE scenarios)

Silence PvE is not a declared dependency. This addon does not call Silence APIs.

## Configuration

On its first server start, the addon creates:

```text
$profile:RHSSilencePvEFactionLock/settings.json
```

Use the following format. JSON does not allow comments or trailing commas.

```json
{
  "disabledFactions": [
    "RHS_AFRF",
    "FIA"
  ]
}
```

Set `disabledFactions` to an empty array to make no faction changes:

```json
{
  "disabledFactions": []
}
```

Restart or reload the scenario after editing this file. The addon reads it at scenario start.

## Behaviour and scope

The RHS Conflict scenario can disable runtime changes to faction playability. To apply the JSON configuration, this addon enables that built-in manager option on the server, then calls the game API to mark only the configured factions as non-playable. It does not choose or enable a player faction; that remains the scenario's responsibility.

This affects faction selection and respawn availability. It does not change AI ownership, base layouts, loadouts, arsenals, or mission objectives.

## Development

Open the project in Enfusion Workbench and run **Script Editor → Validate Scripts** (`F7`) before publishing. Test on the intended server scenario; the standalone raw RHS world is not a reliable test environment because it can lack the campaign-base setup expected by Conflict.

## License

MIT. This project is independent and is not affiliated with Bohemia Interactive, RHS, or Silence PvE.
