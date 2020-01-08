# agsimgui

[![Build Status](https://dev.azure.com/ericoporto/agsimgui/_apis/build/status/ericoporto.agsimgui?branchName=master)](https://dev.azure.com/ericoporto/agsimgui/_build/latest?definitionId=12&branchName=master)

Dear ImGui plugin for Adventure Game Studio


![](https://user-images.githubusercontent.com/2244442/71694439-f3ac1080-2d8d-11ea-9c98-e6954409b66b.gif)

Using ocornut Dear ImGui, and also using software renderer with modifications from LAK132, and using other changes to ensure AGS compatibility.

Additionally using David Cappelo clip library for providing clipboard integration.

## Usage

Run a simple demo window to see what's possible with Dear ImGui.

```AGS Script
// use function room_RepExec() when in Room Script and link it throught the editor
void repeatedly_execute(){
    AgsImGui.NewFrame(); //let's begin a new frame, we end it with a Render
    AgsImGui.ShowDemoWindow(); //Shows a demo of everything possible
    AgsImGui.Render(); // This will generate drawing instructions. 
    // AGS will actually draw this on screen later on, on Post Screen Draw stage.
}
```

Some of what is shown on Demo Window is not yet exposed in the AgsImGui Script API.

Let's do a simple example now.

```AGS Script
bool is_button_clicked;

// use function room_RepExec() when in Room Script and link it throught the editor
void repeatedly_execute(){
    AgsImGui.NewFrame(); //let's begin a new frame, we end it with a Render
	
    AgsImGui.BeginWindow("My first window");	
    ViewFrame* vf = Game.GetViewFrame(player.View, player.Loop, player.Frame);
    is_button_clicked = AgsImGui.ImageButton(vf.Graphic); 
    if(AgsImGui.IsItemHovered()) AgsImGui.SetTooltip(String.Format("frame %d",player.Frame));
    player.x = AgsImGui.DragInt("player.x", player.x);
    player.y = AgsImGui.DragInt("player.y", player.y);
    AgsImGui.EndWindow();
	
    AgsImGui.Render(); // This will generate drawing instructions. 
    // AGS will actually draw this on screen later on, on Post Screen Draw stage.
}
```

Note ImGui will save a `ImGui.ini` to allow window positions and some more data to persist between sections when 
no guidance is given.

## AGS Script API

### Main

#### `AgsImGui.NewFrame`

`static void AgsImGui.NewFrame()`

Call this before calling any AgsImGui commands.

#### `AgsImGui.EndFrame`

`static void AgsImGui.EndFrame()`

We don't need this if we are using Render, since it will automatically call `AgsImGui.EndFrame()` too.

#### `AgsImGui.Render`

`static void AgsImGui.Render()`

This will EndFrame and proceed to generate drawing instructions.

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

---

### Widgets: Text

#### `AgsImGui.Text`

`static void AgsImGui.Text(String text)`

Draws a string of text.

#### `AgsImGui.TextColored`

`static void AgsImGui.TextColored(int ags_color, String text)`

shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();

#### `AgsImGui.TextDisabled`

`static void AgsImGui.TextDisabled(String text)`

shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor(); 

#### `AgsImGui.TextWrapped`

`static void AgsImGui.TextWrapped(String text)`

shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();.

#### `AgsImGui.LabelText`

`static void AgsImGui.LabelText(String label, String text)`

Display text+label aligned the same way as value+label widgets .

#### `AgsImGui.Bullet`

`static void AgsImGui.Bullet(String text)`

Draws a bullet and a string of text. Shortcut for Bullet()+Text().

---

### Widgets: Main

Most widgets return true when the value has been changed or when pressed/selected.

You may also use one of the many IsItemXXX functions (e.g. IsItemActive, IsItemHovered, etc.) to query widget state.

#### `AgsImGui.Button`

`static bool AgsImGui.Button(String label, int width = 0, int height = 0)`

Creates a button. Returns true while button is pressed.

#### `AgsImGui.SmallButton`

`static bool AgsImGui.SmallButton(String label)`

Creates a button with no padding to be easier to embed within text. Returns true while button is pressed.

#### `AgsImGui.Image`

`static void AgsImGui.Image(int sprite_id)`

Create an image with passed sprite ID.

#### `AgsImGui.ImageButton`

`static bool AgsImGui.ImageButton(int sprite_id)`

Create a button with an image with passed sprite ID. Returns true while button is pressed.

#### `AgsImGui.ArrowButton`

`static bool AgsImGui.ArrowButton(String label, ImGuiDir dir)`

Creates a button with an arrow shape. Returns true while button is pressed.

#### `AgsImGui.Checkbox`

`static bool AgsImGui.Checkbox(String label, bool initial_value)`

Creates a checkbox button. Returns true when button is marked.

#### `AgsImGui.RadioButton`

`static bool AgsImGui.RadioButton(String label, bool active)`

Creates a radio button. Returns true while button is marked.

#### `AgsImGui.Bullet`

`static void AgsImGui.Bullet()`

Draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses .

---

### Widgets: Selectables

A selectable highlights when hovered, and can display another color when selected.

#### `AgsImGui.Selectable`

`static bool AgsImGui.Selectable(String label, bool selected = false, ImGuiSelectableFlags flags = 0, int width = 0, int height = 0)`

bool selected carry the selection state (read-only). When Selectable() is clicked it returns true so you can modify your selection state.

---

### Widgets: Drag

CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds. 

#### `AgsImGui.DragFloat`

`static float AgsImGui.DragFloat(String label, float value, float min_value = 0, float max_value = 0, float speed = 0, String format = 0)`

Returns the current value of the drag box. Format string uses regular `"%f"` specifiers, so you can use to define how many decimals you want.

#### `AgsImGui.DragInt`

`static int AgsImGui.DragInt(String label, int value, int min_value = 0, int max_value = 0, float speed = 0, String format = 0)`

Returns the current value of the drag box. Format string uses regular `"%d"`, you can use it to specify left most zeroes.

---

### Widgets: Slider

CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds. 

#### `AgsImGui.SliderFloat`

`static float AgsImGui.SliderFloat(String label, float value, float min_value = 0, float max_value = 0, String format = 0)`

Returns the current value of the slider. Format string uses regular `"%f"` specifiers, so you can use to define how many decimals you want.

#### `AgsImGui.SliderInt`

`static int AgsImGui.SliderInt(String label, int value, int min_value = 0, int max_value = 0, String format = 0)`

Returns the current value of the slider. Format string uses regular `"%d"`, you can use it to specify left most zeroes.

---

### Widgets: Input with Keyboard

#### `AgsImGui.InputText`

`static String AgsImGui.InputText(String label, String text_buffer, ImGuiInputTextFlags flags =0)`

#### `AgsImGui.InputTextMultiline`

`static String AgsImGui.InputTextMultiline(String label, String text_buffer, int width=0, int height=0, ImGuiInputTextFlags flags = 0)`

#### `AgsImGui.InputTextWithHint`

`static String AgsImGui.InputTextWithHint(String label, String hint, String text_buffer, ImGuiInputTextFlags flags = 0)`

---

### Widgets: Combobox commands

#### `AgsImGui.BeginCombo`

`static bool AgsImGui.BeginCombo(String label, String preview_value, ImGuiComboFlags flags = 0);`

The BeginCombo()/EndCombo() allows to manage your contents and selection state however you want it, by creating e.g. Selectable() items. 

Example:

```
	bool option1;
	bool option2;
	if(AgsImGui.BeginCombo("My combo","click me!")){
		option1 = AgsImGui.Selectable("Option 1");
		option2 = AgsImGui.Selectable("Option 2");
		AgsImGui.EndCombo();
	}
```

#### `AgsImGui.EndCombo`

`static void EndCombo()`

Only call EndCombo() if BeginCombo() returns true!

---

### Widgets: List Boxes commands

#### `AgsImGui.BeginListBox`

`static bool AgsImGui.BeginListBox(String label, int items_count, int height_in_items = -1);`

The BeginListBox()/EndListBox() allows to manage your contents and selection state however you want it, by creating e.g. Selectable() items. 

Example:

```
	bool option1;
	bool option2;
	if(AgsImGui.BeginListBox("My ListBox",2)){
		option1 = AgsImGui.Selectable("Option 1");
		option2 = AgsImGui.Selectable("Option 2");
		AgsImGui.EndListBox();
	}
```

#### `AgsImGui.EndListBox`

`static void AgsImGui.EndListBox()`

Only call EndListBox() if BeginListBox() returns true!

---

### Tooltips

Tooltip are windows following the mouse which do not take focus away.

Remember we can only have one active tooltip at all times, and the last one called is the active one.

#### `AgsImGui.BeginTooltip`

`static void AgsImGui.BeginTooltip()`

Begin/append a tooltip window. to create full-featured tooltip (with any kind of items). Doesn't return nothing.

#### `AgsImGui.EndTooltip`

`static void AgsImGui.EndTooltip()`

Always call after a BeginTooltip block!

#### `AgsImGui.SetTooltip`

`static void AgsImGui.SetTooltip(String text)`

Set a text-only tooltip, typically use with AgsImGui.IsItemHovered(). Override any previous call to SetTooltip().

---

### Tab Bars, Tabs

#### `AgsImGui.BeginTabBar`

`static bool AgsImGui.BeginTabBar(String str_id, ImGuiTabBarFlags flags = 0)`

create and append into a TabBar

#### `AgsImGui.EndTabBar`

`static void AgsImGui.EndTabBar()`

only call EndTabBar() if BeginTabBar() returns true!

#### `AgsImGui.BeginTabItem`

`static bool AgsImGui.BeginTabItem(String label, bool has_close_button = 0, ImGuiTabItemFlags flags = 0)`

create a Tab. Returns true if the Tab is selected.

#### `AgsImGui.EndTabItem`

`static void AgsImGui.EndTabItem()`

only call EndTabItem() if BeginTabItem() returns true!

#### `AgsImGui.SetTabItemClosed`

`static void AgsImGui.SetTabItemClosed(String tab_or_docked_window_label)`

notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.

---

### Menus 

#### `AgsImGui.BeginMenuBar`

`static bool AgsImGui.BeginMenuBar()`

Append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window).

#### `AgsImGui.EndMenuBar`

`static void AgsImGui.EndMenuBar()`

Only call EndMenuBar() if BeginMenuBar() returns true!

#### `AgsImGui.BeginMainMenuBar`

`static bool AgsImGui.BeginMainMenuBar()`

Create and append to a full screen menu-bar.

#### `AgsImGui.EndMainMenuBar`

`static void AgsImGui.EndMainMenuBar()`

Only call EndMainMenuBar() if BeginMainMenuBar() returns true!

#### `AgsImGui.BeginMenu`

`static bool AgsImGui.BeginMenu(String label, bool enabled = true)`

Create a sub-menu entry. Only call EndMenu() if this returns true!

#### `AgsImGui.EndMenu`

`static void AgsImGui.EndMenu()`

Only call EndMenu() if BeginMenu() returns true!

#### `AgsImGui.MenuItem`

`static bool AgsImGui.MenuItem(String label, String shortcut, bool selected = false, bool enabled = true)`

return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment!

---

### General Helpers

#### `AgsImGuiHelper.SetClipboarText` 

`static void AgsImGuiHelper.SetClipboarText(String text)`

#### `AgsImGuiHelper.GetClipboarText`

`static String AgsImGuiHelper.GetClipboarText()`

---

### Enums

#### `ImGuiCond`

- `eImGuiCond_Always`, used when condition should always trigger, is usually the default when the AgImGui API requires 
conditions.

- `eImGuiCond_Once`, Set the variable once per runtime session (only the first call with succeed).

- `eImGuiCond_FirstUseEver`, Set the variable if the object/window has no persistently saved data (no entry in .ini file).

- `eImGuiCond_Appearing`, Set the variable if the object/window is appearing after being hidden/inactive (or the first time).


#### `ImGuiDir`

- `eImGuiDir_Left`, left direction.

- `eImGuiDir_Right`, right direction.

- `eImGuiDir_Up`, up direction.

- `eImGuiDir_Down`, down direction.

#### `ImGuiBeginWindow`

obs: This is exclusive to AgsImGui implementation because in AGS we don't have pointers to primitive types.

In any cases below, you still have to call `AgsImGui.EndWindow()`.

- `eImGuiBeginWindow_OK`, the window is open.

- `eImGuiBeginWindow_Collapsed`, the window is collapsed (using the arrow at top left).

- `eImGuiBeginWindow_OK_Closed` the window is closed.

- `eImGuiBeginWindow_Collapsed_Closed` the window is both collapsed and closed, rare but theoretically possible.

## FAQ

### How do I make mouse wheel work?

First, make sure you have mouse wheel support enabled in Global Settings.

Then, in some script, add the code below. Note: If the script is a Room script, it will only work in that room.

```AGS Script
void on_mouse_click (int btn){
  if(btn == eMouseWheelNorth){
    AgsImGui.DoMouseWheel(eImGuiDir_Down);
  } else if(btn == eMouseWheelSouth){
    AgsImGui.DoMouseWheel(eImGuiDir_Up);    
  }  
}
```

For now, mouse wheel events are not exposed in the AGS Plugin API, so you have to do it through script. In the future, if this changes, it will be integrated in the plugin.

