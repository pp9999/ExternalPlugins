# ExternalPluginsMM

ExternalPluginsMM is the standalone x64 plugin project for the current
MemoryError host. It provides the host-facing SDK facade, bundled C++ scripts,
and the plugin UI/lifecycle entry points.

## Host compatibility

The declarations in `ExternalPlugins/source/API.h`, `Structs.h`, and the domain
headers mirror the public ABI exposed by the current MemoryError DLL. Keep
those headers synchronized with the host whenever exported functions or
cross-DLL structs change. The project links against the host's generated
`XInput1_4_inject.lib`; `ExternalPlugins/MemoryError.def` is only a historical
binary dump and is not the API source of truth.

The plugin exports these unmangled lifecycle functions:

- `GetScripts`
- `LoadDLL(HWND)`
- `RenderUI()`
- `onUnload()`
- `UnloadRequested()`

The host also requires `PluginName` and `PluginAuthor` version-resource fields
for discovery.

## Build output

The x64 post-build step copies the DLL to:

```text
%USERPROFILE%\MemoryError\Plugins\ExternalPlugins.dll
```

This matches the default plugin directory used by MemoryError. Final Visual
Studio builds and live plugin loading must be performed manually by the
developer; the repository agent must not build or inject the DLL.

Original project community: https://discord.gg/ywSDHbXmF5
