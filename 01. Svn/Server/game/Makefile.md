# Server build notes

`title_system.cpp` is a new game source file. It must be added to the build list.

## FreeBSD / Makefile

`Srcs/Server/game/src/Makefile`, in the `CPPFILE` list:

```make
	mount_up_grade.cpp passive_attr.cpp title_system.cpp GroupMatchManager.cpp MonstercardSystem.cpp npc_location_helper.cpp treasure_hunt_manager.cpp questlua_treasure_hunt.cpp \
```

## Visual Studio

`Srcs/Server/game/src/game.vcxproj`:

```xml
    <ClCompile Include="title_system.cpp" />
    <ClInclude Include="title_system.h" />
```

After adding the file run a clean build. If `title_system.cpp` is missing from the build
list you will get unresolved external symbol errors on `CTitleSystemManager`.
