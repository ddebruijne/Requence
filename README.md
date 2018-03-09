# Requence
Requence is a work-in-progress Unreal Engine 4 plugin that takes care of all functionality required to load and apply custom key bindings. Originally developed for Impeller Studio's Starfighter Inc's requirement of extremely customizable input - this can be used in any UE4 project to remove the development hassle that is input. Instead only focus on the UI, and let Requence do the heavy lifting!

## Current Features
- Rebinding, deleteing and adding both Axis and Action key bindings.
- Device-seperated input mappings, supporting multiple bindings for a branch
- Import & Export device presets with JSON
- Reset to default

## Planned
- Unique device support - Mainly Joysticks - Using SDL to detect devices like the Thrustmaster T.16000
- Steam Workshop Support
- Curve Editor
- Deadzone & falloff
- Key detection functionality in C++, instead of relying on BP

## Bugs to fix
- Full Axis/Action list now only fills at loading of unreal input. this should be moved to LoadInput, and be filtered out with SaveInput. This functionality should extend to JSON import/exports so we don't export unneccesary data, and add the empty ones back in.
- Full Axis/Action list should be updated with OnGameStartup