# Building foo_discogger

The project builds with Visual Studio 2022 (toolset v143, C++17) on Windows.
`.github/workflows/build.yml` does all of the steps below automatically and is
the reference if anything here is unclear.

## Directory layout

`foo_discogger.vcxproj` uses relative paths, so the checkout has to sit inside
the foobar2000 SDK:

```
<root>\WTL10_9163\Include\                 WTL 10 headers (NuGet package "wtl" 10.0.10320)
<root>\sdk\                                foobar2000 SDK 2025-03-07 (SDK-2025-03-07.7z)
<root>\sdk\pfc\  <root>\sdk\libPPUI\
<root>\sdk\foobar2000\{SDK,helpers,shared,foobar2000_component_client}\
<root>\sdk\foobar2000\foo_discogger\        this repository
<root>\sdk\foobar2000\discogger libs\sqlite-amal-3401\include\sqlite3.h
```

The release v1.0.22.3 was built with SDK 2025-03-07 (`fb2k SDK: 20250307 80`).
Newer SDKs require C++20 and changed the `cfg_objList` API used by `conf.cpp`.
The SDK projects `helpers` and `libPPUI` also need WTL in their include path
(the CI adds it with a generated `Directory.Build.targets`).

## Third-party files (not in the repository)

| Library | Put into |
|---|---|
| jansson 2.14, built with `HAVE_UNISTD_H` (see `jansson/how to json_loadfd.txt`) | `jansson\jansson.h`, `jansson\jansson_config.h`; `jansson.lib` in `<Platform>\<Configuration>\` |
| liboauthcpp (github.com/sirikata/liboauthcpp) | `liboauthcpp\liboauthcpp.h`; `oauthcpp.lib` in `<Platform>\<Configuration>\` |
| zlib headers (zlib1.dll is loaded at runtime) | `zlib\zlib.h`, `zlib\zconf.h` |
| SQLite 3.40.1 amalgamation, compiled with `/FI sqlite3ren.h` (symbols renamed to `discogger_*`) | `..\disgogger libs\sqlite-amal-336\<Platform>\<Configuration>\sqlite3.lib` (x64 Release: `..\discogger libs\...`) |

## Build

```
msbuild foo_discogger.vcxproj /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143 /p:SolutionDir=<root>\sdk\foobar2000\foo_discogger\
```

Some configurations of the SDK projects (`libPPUI`, `helpers`) still select
toolset v142. `/p:PlatformToolset=v143` builds everything with the same
toolset; without it the v142 build tools must be installed together with
their ATL component.

## Reconstructed sources

Some files were referenced upstream but never committed. They were
reconstructed here (see the git history for details): `db_fetcher.h`, `crc.h`
(standard CRC-32), `foo_discogs_threaded_locked_process_callback` (tasks.h),
`SKIP_RELEASE_DLG_VA_AUTO_LOAD` (conf.h), `is_multivalue_meta` (utils.cpp) and
`Release::get_query_major_formats_qty` (discogs.h).
