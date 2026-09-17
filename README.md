# RHS Silence PvE Faction Lock

A small server-side addon for RHS Conflict scenarios in Arma Reforger.

Use it when you want to keep particular factions AI-only. It hides those factions from the faction-selection and respawn menus, while leaving the scenario in charge of the faction players actually use.

Out of the box, it disables `RHS_AFRF` and `FIA`. That makes it a useful fit for a US-focused RHS / Silence PvE server, without hard-coding USAF as the player faction.

## What you need

- Arma Reforger
- RHS - Status Quo
- RHS - Content Pack 01
- RHS - Content Pack 02
- A scenario based on `SCR_GameModeCampaign` — compatible Silence PvE scenarios are included in that group

Silence PvE is not a dependency. The addon does not call Silence APIs, so it can also be used with another compatible RHS Conflict scenario.

## Configuration

On the first server start, the addon creates this file in the server profile:

```text
$profile:RHSSilencePvEFactionLock/settings.json
```

Edit the list to choose which faction keys should be unavailable to players:

```json
{
  "disabledFactions": [
    "RHS_AFRF",
    "FIA"
  ]
}
```

To keep the addon installed but make no changes, use an empty list:

```json
{
  "disabledFactions": []
}
```

Restart or reload the scenario after changing the file. The settings are read when the scenario starts. JSON does not support comments or trailing commas.

## What it changes

Only faction availability. The configured factions become non-playable, so players cannot select or respawn as them.

It does not alter AI ownership, bases, objectives, loadouts, arsenals, or which faction the scenario treats as the player side. Those remain in the scenario configuration.

The addon enables the Conflict faction manager's built-in runtime setting on the server, then uses the game API to apply the configured list. Keeping this behaviour in a small, open-source addon makes it easier to audit and adjust for a specific server.

## Developing or testing

Open the project in Enfusion Workbench and run **Script Editor → Validate Scripts** (`F7`) before publishing changes.

Test with the scenario you intend to host. Opening the raw RHS world by itself in Workbench is not a reliable test: it can be missing the campaign-base setup expected by Conflict.

## License

MIT. This project is independent and is not affiliated with Bohemia Interactive, RHS, or Silence PvE.
