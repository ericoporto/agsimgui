# agsimgui

[![Build Status](https://dev.azure.com/ericoporto/agsimgui/_apis/build/status/ericoporto.agsimgui?branchName=master)](https://dev.azure.com/ericoporto/agsimgui/_build/latest?definitionId=12&branchName=master)

Dear ImGui plugin for Adventure Game Studio


![](https://user-images.githubusercontent.com/2244442/71694439-f3ac1080-2d8d-11ea-9c98-e6954409b66b.gif)

Using ocornut Dear ImGui, and also using software renderer with modifications from LAK132, and using other changes to ensure AGS compatibility.

Additionally using David Cappelo clip library for providing clipboard integration.

## AGS Script API

### Main

#### `AgsImGui.NewFrame`

`static void AgsImGui.NewFrame()`


#### `AgsImGui.EndFrame`

`static void AgsImGui.EndFrame()`


#### `AgsImGui.Render`

`static void AgsImGui.Render()`

---

### Demo, Debug, Information

#### `AgsImGui.GetVersion`

`static String AgsImGui.GetVersion()`

#### `AgsImGui.ShowDemoWindow`

`static void AgsImGui.ShowDemoWindow()`

#### `AgsImGui.ShowAboutWindow`

`static void AgsImGui.ShowAboutWindow()`

#### `AgsImGui.ShowMetricsWindow`

`static void AgsImGui.ShowMetricsWindow()`

#### `AgsImGui.ShowUserGuide`

`static void AgsImGui.ShowUserGuide()`

---

### Windows

#### AgsImGui.BeginWindow

`static AgsImGui.ImGuiBeginWindow BeginWindow(String name, bool has_close_button = 0, ImGuiWindowFlags flags = 0)`

#### AgsImGui.EndWindow

`static void EndWindow()`

---

### Child Windows

#### `AgsImGui.BeginChild`

`static bool BeginChild(String str_id, int width = 0, int height = 0, bool border = false, ImGuiWindowFlags flags = 0)`

Child Windows. Always call a matching EndChild() for each BeginChild() call, regardless of its return value. Child windows can embed their own child.

#### `AgsImGui.EndChild`

`static void EndChild()`

Pop child window from the stack.

---

### Item/Widgets Utilities

Most of the functions are referring to the last/previous item we submitted.

#### `AgsImGui.IsItemHovered`

`static bool AgsImGui.IsItemHovered(ImGuiHoveredFlags flags = 0)`

Is the last item hovered? (and usable, aka not blocked by a popup, etc.). You can specify a flag for more options.

static bool IsItemHovered(ImGuiHoveredFlags flags = 0)

#### `AgsImGui.IsItemActive`

`static bool AgsImGui.IsItemActive()`

Is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item.)

#### `AgsImGui.IsItemFocused`

`static bool AgsImGui.IsItemFocused()`

Is the last item focused for keyboard navigation?

#### `AgsImGui.IsItemVisible`

`static bool AgsImGui.IsItemVisible()`

Is the last item visible? (items may be out of sight because of clipping/scrolling)

#### `AgsImGui.IsItemEdited`

`static bool AgsImGui.IsItemEdited()`

Did the last item modify its underlying value this frame? or was pressed? This is generally the same as the bool return value of many widgets.

#### `AgsImGui.IsItemActivated`

`static bool AgsImGui.IsItemActivated()`

Was the last item just made active (item was previously inactive).

#### `AgsImGui.IsItemDeactivated`

`static bool AgsImGui.IsItemDeactivated()`

Was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.

#### `AgsImGui.IsItemDeactivatedAfterEdit`

`static bool AgsImGui.IsItemDeactivatedAfterEdit()`

Was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing.

#### `AgsImGui.IsItemToggledOpen`

`static bool AgsImGui.IsItemToggledOpen()`

Was the last item open state toggled? set by TreeNode().

#### `AgsImGui.IsAnyItemHovered`

`static bool AgsImGui.IsAnyItemHovered()`

is any item hovered?

#### `AgsImGui.IsAnyItemActive`

`static bool AgsImGui.IsAnyItemActive()`

is any item active?

#### `AgsImGui.IsAnyItemFocused`

`static bool AgsImGui.IsAnyItemFocused()`

is any item focused?

#### `AgsImGui.IsWindowAppearing`

`static bool AgsImGui.IsWindowAppearing()`

'current window' = the window we are appending into while inside a Begin()/End() block.

#### `AgsImGui.IsWindowCollapsed`

`static bool AgsImGui.IsWindowCollapsed()`

return true when window is collapsed. Use this between Begin and End of a window.

#### `AgsImGui.IsWindowFocused`

`static bool AgsImGui.IsWindowFocused(ImGuiFocusedFlags flags=0)`

is current window focused? or its root/child, depending on flags. see flags for options. Use this between Begin and End of a window.

#### `AgsImGui.IsWindowHovered`

`static bool AgsImGui.IsWindowHovered(ImGuiHoveredFlags flags=0)`

is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. Use this between Begin and End of a window.
