# UserInterface project notes

Add these files to the UserInterface project and filters if your fork uses Visual Studio project files:

```txt
PythonTitleSystem.cpp
PythonTitleSystem.h
```

The current source tree already had these entries in:

```txt
Srcs/Client/vs_files/UserInterface/UserInterface.vcxproj
```

If your target fork does not have them, add `PythonTitleSystem.cpp` as `ClCompile` and
`PythonTitleSystem.h` as `ClInclude`.

The title preview panel renders into a dedicated render target. Your client must already
support `RENDER_TARGET` / `wndMgr.RegisterRenderTarget`. If it does not, port the render
target widget first, otherwise `uiscript/charactertitlewindow.py` will fail on
`model_render_target`.
