# Faction Availability Lock

A small, server-side faction availability control for Arma Reforger Conflict-style scenarios.

Use it when you want to keep specific factions AI-only. It hides those factions from faction-selection and respawn menus, while leaving the scenario in charge of the player faction.

## What you need

- Arma Reforger
- A scenario based on `SCR_GameModeCampaign`

The addon depends only on Arma Reforger's base Conflict API. It has no RHS, Silence PvE, or third-party mod dependency.

## Configuration

On the first server start, the addon creates this file in the server profile:

```text
$profile:FactionAvailabilityLock/settings.json
```

Edit the list to choose which faction keys should be unavailable to players:

```json
{
  "disabledFactions": []
}
```

An empty list makes no changes. Add the faction keys used by your scenario to make them unavailable to players:

```json
{
  "disabledFactions": [
    "FactionKeyToDisable"
  ]
}
```

Restart or reload the scenario after changing the file. The settings are read when the scenario starts. JSON does not support comments or trailing commas.

## What it changes

Only faction availability. The configured factions become non-playable, so players cannot select or respawn as them.

It does not alter AI ownership, bases, objectives, loadouts, arsenals, or which faction the scenario treats as the player side. Those remain in the scenario configuration.

The addon enables the Conflict faction manager's built-in runtime setting on the server, then uses the game API to apply the configured list. Keeping this behaviour in a small, open-source addon makes it easier to audit and adjust for a specific server.

All clients joining a server download the addon automatically. “Server-side” here means that the faction-changing logic runs only on the server.

## Developing or testing

Open the project in Enfusion Workbench and run **Script Editor → Validate Scripts** (`F7`) before publishing changes.

Test with the scenario you intend to host. A raw world opened on its own in Workbench can be missing the campaign-base setup expected by Conflict.

## License

[Arma Public License (APL)](https://www.bohemia.net/en/licenses/arma-public-license). This project is independent and is not affiliated with Bohemia Interactive or any third-party mod team.
