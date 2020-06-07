# agsimgui

[![Build Status](https://dev.azure.com/ericoporto/agsimgui/_apis/build/status/ericoporto.agsimgui?branchName=master)](https://dev.azure.com/ericoporto/agsimgui/_build/latest?definitionId=12&branchName=master)

Dear ImGui plugin for Adventure Game Studio


![](https://user-images.githubusercontent.com/2244442/71694439-f3ac1080-2d8d-11ea-9c98-e6954409b66b.gif)

Using ocornut Dear ImGui, and also using software renderer with modifications from LAK132, and using other changes to ensure AGS compatibility.

ImGui reference commit is 3f26a07ee1813cecaa87253436149e28fc11dc4e.

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

#### `AgsImGui.GetStyle`

`static ImGuiStyle* GetStyle()`

Gets the Style customization use in AgsImGui. Check [`ImGuiStyle`](#imguistyle) for more information.

#### `AgsImGui.SetStyle`

`static void SetStyle(ImGuiStyle* imGuiStyle)`

Sets the Style customization use in AgsImGui. Check [`ImGuiStyle`](#imguistyle) for more information.

```AGS Script
ImGuiStyle* style = AgsImGui.GetStyle();
style.Alpha = 0.5
style.Colors[eImGuiCol_PopupBg] = ImVec4.Create(1.00, 1.00, 1.00, 1.00); //white 
AgsImGui.SetStyle(style);
```

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

`static String AgsImGui.InputText(String label, String text_buffer, int buffer_size, ImGuiInputTextFlags flags =0)`

Pass a string to identify the input field as label, this label is going to be used for ID and can't be empty.

Remember that we don't save state between frames, so you have to do this yourself by reapplying the output to input as needed.

This function returns `null` when the input hasn't changed, and returns a string with all the text, when it has been modified.

This widget supports Ctrl+Z for undo, Ctrl+X for Cut, Ctrl+C for copy, Ctrl+V for pasting, mouse selection and cursor navigation with arrows. 
Behavior can be modified by passing flags, see the enums section. Flags can be combined with bitwise operations.

Example:
```AGS Script
String input_string;
function room_RepExec() {   
  AgsImGui.NewFrame();  
    // buff
  String out_string = AgsImGui.InputText("type here!",input_string, 128);
  if(out_string!=null) input_string = out_string;
  
  AgsImGui.Render();
}


function room_Load() {
  input_string = "Hello String!";
}
```


#### `AgsImGui.InputTextMultiline`

`static String AgsImGui.InputTextMultiline(String label, String text_buffer, int buffer_size, int width=0, int height=0, ImGuiInputTextFlags flags = 0)`

Same as InputText, but allows controlling the input size and supports multiline text.

#### `AgsImGui.InputTextWithHint`

`static String AgsImGui.InputTextWithHint(String label, String hint, String text_buffer, int buffer_size, ImGuiInputTextFlags flags = 0)`

Same as InputText, but supports passing an input hint text.

---

### Widgets: Combobox commands

#### `AgsImGui.BeginCombo`

`static bool AgsImGui.BeginCombo(String label, String preview_value, ImGuiComboFlags flags = 0);`

The BeginCombo()/EndCombo() allows to manage your contents and selection state however you want it, by creating e.g. Selectable() items. 

Example:

```AGS Script
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

```AGS Script
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

### Widgets: Trees

TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.

#### `AgsImGui.TreeNode`

`static bool AgsImGui.TreeNode(String label)`

Show the contents and call TreePop() at the end, if this returns true.

#### `AgsImGui.TreeNodeWithID`

`static bool AgsImGui.TreeNodeWithID(String str_id, String text)`

Show the contents and call TreePop() at the end, if this returns true.

#### `AgsImGui.TreeNodeV`

`static bool AgsImGui.TreeNodeV(String str_id, String text)`

Show the contents and call TreePop() at the end, if this returns true.

#### `AgsImGui.TreeNodeEx`

`static bool AgsImGui.TreeNodeEx(String label, ImGuiTreeNodeFlags flags = 0)`

Show the contents and call TreePop() at the end, if this returns true.

#### `AgsImGui.TreeNodeExWithID`

`static bool AgsImGui.TreeNodeExWithID(String str_id, ImGuiTreeNodeFlags flags, String text)`

Show the contents and call TreePop() at the end, if this returns true.

#### `AgsImGui.TreeNodeExV`

`static bool AgsImGui.TreeNodeExV(String str_id, ImGuiTreeNodeFlags flags, String text)`

Show the contents and call TreePop() at the end, if this returns true.

#### `AgsImGui.TreePush`

`static void AgsImGui.TreePush(String str_id)`

~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.

#### `AgsImGui.TreePop`

`static void AgsImGuiTreePop()`

~ Unindent()+PopId().

#### `AgsImGui.GetTreeNodeToLabelSpacing`

`static float AgsImGui.GetTreeNodeToLabelSpacing()`

Horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode.

#### `AgsImGui.CollapsingHeader`

`static bool AgsImGui.CollapsingHeader(String label, ImGuiTreeNodeFlags flags = 0)`

If returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().

#### `AgsImGui.SetNextItemOpen`

`static void AgsImGui.SetNextItemOpen(bool is_open, ImGuiCond cond = 0)`

Set next TreeNode/CollapsingHeader open state.

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

Example:

```AGS Script
AgsImGui.Button("Clicks for nothing!");
if(AgsImGui.IsItemHovered()) AgsImGui.SetTooltip("Button is hovered!");

```

---

### Popups and Modals

#### `AgsImGui.OpenPopup`

`static void AgsImGui.OpenPopup(String str_id)`

call to mark popup as open (don't call every frame!). Popups are closed when user click outside, or if CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block. 

Popup identifiers are relative to the current ID-stack.

Example:

```AGS Script
// If the button is in the Window, this code goes inside the window.
if(AgsImGui.BeginPopup("my_popup")){
  AgsImGui.Text("This is a popup");
  AgsImGui.EndPopup();  
}
if(AgsImGui.Button("open popup"))
{
  AgsImGui.OpenPopup("my_popup");
}
```

#### `AgsImGui.BeginPopup`

`static bool AgsImGui.BeginPopup(String str_id, ImGuiWindowFlags flags = 0)`

Return true if the popup is open, and you can start outputting to it. 
Only call EndPopup() if BeginPopup() returns true!

Generally you will want to run this on every frame, and it will return true once the popup has been made open, 
and return false again once it's closed.

#### `AgsImGui.BeginPopupModal`

`static bool AgsImGui.BeginPopupModal(String name, bool has_close_button = 0, ImGuiWindowFlags flags = 0)`

 Modal dialog, a regular window with title bar, block interactions behind the modal window, and you can't close the 
 modal window by clicking outside. 

#### `AgsImGui.EndPopup`

`static void AgsImGui.EndPopup()`

Only call EndPopup() if BeginPopupXXX() returns true!

#### `AgsImGui.IsPopupOpen`

`static bool AgsImGui.IsPopupOpen(String str_id)`

Return true if the popup is open at the current begin-ed level of the popup stack.

#### `AgsImGui.CloseCurrentPopup`

`static void AgsImGui.CloseCurrentPopup()`

Close the popup we have begin-ed into. clicking on a MenuItem or Selectable automatically close the current popup.

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

### ImVec2

This is a format used to describe either x,y points or zero origin rectangles and other things that can be described as 
vectors. It's used more in the attributes of a `ImGuiStyle` object.

#### `ImVec2.Create`

`static ImVec2* ImVec2.Create(float x = 0, float y = 0)`

Creates a new ImVec2 object. You can pass float variable for it's `X` and `Y` properties.

#### `ImVec2.X`

`attribute float ImVec2.X`

It's the X attribute of a ImVec2 object. Usually represents the horizontal axis.

#### `ImVec2.Y`

`attribute float ImVec2.Y`

It's the Y attribute of a ImVec2 object. Usually represents the vertical axis.

#### `ImVec2.Scale`

`ImVec2* ImVec2.Scale(float scale)`

Multiplies x and y coordinates by a scalar and returns a new ImVec2 with the result.

#### `ImVec2.Length`

`float ImVec2.Length()`

Returns length from ImVec2 (distance from 0,0 origin).

#### `ImVec2.SquaredLength`

`float ImVec2.SquaredLength()`

Returns squared length from ImVec2 (distance from 0,0 origin). Faster than length.

#### `ImVec2.Add`

`ImVec2* ImVec2.Add(ImVec2* imVec2)`

Returns a new ImVec2 with the sum of this with imVec2.

#### `ImVec2.Sub`

`ImVec2* ImVec2.Sub(ImVec2* imVec2)`

Returns a new ImVec2 with the subtraction of imVec2 from this. 

---

### ImVec4

This is a format used to describe either x,y,w,z points or any origin rectangles and other things that can be described 
as vectors with 4 coordinates. It's the preferable format for ImGui Colors, and used a lot for this! It's used more in 
the attributes of a `ImGuiStyle` object.

#### `ImVec4.Create`

`static ImVec4* ImVec4.Create(float x = 0, float y = 0, float z = 0, float w = 0)`

Creates a new ImVec4 object. You can pass float variable for it's `X`, `Y`, `Z` and `W` properties.

#### `ImVec4.X`

`attribute float ImVec4.X`

It's the X attribute of a ImVec4 object. Usually represents the horizontal axis. 
When used for colors correspond to the color Red.

#### `ImVec4.Y`

`attribute float ImVec4.Y`

It's the Y attribute of a ImVec4 object. Usually represents the vertical axis.
When used for colors correspond to the color Green.

#### `ImVec4.Z`

`attribute float ImVec4.Z`

It's the Z attribute of a ImVec4 object. Usually represents the length in the horizontal axis. 
When used for colors correspond to the color Blue.

#### `ImVec4.W`

`attribute float ImVec4.W`

It's the W attribute of a ImVec4 object. Usually represents the length in the vertical axis.
When used for colors correspond to the alpha of the colors, where `0.0` is transparent and `1.0` is opaque.

#### `ImVec4.Scale`

`ImVec4* ImVec4.Scale(float scale)`

Multiplies x, y, z and w coordinates by a scalar and returns a new ImVec4 with the result.

#### `ImVec4.Length`

`float ImVec4.Length()`

Returns length from ImVec4 (distance from 0,0,0,0 origin).

#### `ImVec4.SquaredLength`

`float ImVec4.SquaredLength()`

Returns squared length from ImVec4 (distance from 0,0,0,0 origin). Faster than length.

#### `ImVec4.Add`

`ImVec4* ImVec4.Add(ImVec4* imVec4)`

Returns a new ImVec4 with the sum of this with imVec4.

#### `ImVec4.Sub`

`ImVec4* ImVec4.Sub(ImVec4* imVec4)`

Returns a new ImVec4 with the subtraction of imVec4 from this. 

---

### ImGuiStyle

#### `ImGuiStyle.Create`

`static ImGuiStyle* ImGuiStyle.Create()`

Creates a new empty ImGuiStyle object. Rarely used, it's usually more advised to use `ImGui.GetStyle()` instead, unless it's planned to 
customize ***all*** elements of a ImGuiStyle object. 

#### `ImGuiStyle.Alpha`

`attribute float ImGuiStyle.Alpha`

Global alpha applies to everything in Dear ImGui. 

#### `ImGuiStyle.WindowPadding`

`attribute ImVec2* ImGuiStyle.WindowPadding`

Padding within a window. 

#### `ImGuiStyle.WindowRounding`

`attribute float ImGuiStyle.WindowRounding`

Radius of window corners rounding. Set to 0.0f to have rectangular windows. 

#### `ImGuiStyle.WindowBorderSize`

`attribute float ImGuiStyle.WindowBorderSize`

Thickness of border around windows. Generally set to 0.0f or 1.0f. 

#### `ImGuiStyle.WindowMinSize`

`attribute ImVec2* ImGuiStyle.WindowMinSize`

Minimum window size. This is a global setting. For individual windows, use SetNextWindowSizeConstraints(). 

#### `ImGuiStyle.WindowTitleAlign`

`attribute ImVec2* ImGuiStyle.WindowTitleAlign`

Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered. 

#### `ImGuiStyle.WindowMenuButtonPosition`

`attribute ImGuiDir ImGuiStyle.WindowMenuButtonPosition`

Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir_Left. 

#### `ImGuiStyle.ChildRounding`

`attribute float ImGuiStyle.ChildRounding`

Radius of child window corners rounding. Set to 0.0f to have rectangular windows. 

#### `ImGuiStyle.ChildBorderSize`

`attribute float ImGuiStyle.ChildBorderSize`

Thickness of border around child windows. Generally set to 0.0f or 1.0f. 

#### `ImGuiStyle.PopupRounding`

`attribute float ImGuiStyle.PopupRounding`

Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding) 

#### `ImGuiStyle.PopupBorderSize`

`attribute float ImGuiStyle.PopupBorderSize`

Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. 

#### `ImGuiStyle.FramePadding`

`attribute ImVec2* ImGuiStyle.FramePadding`

Padding within a framed rectangle (used by most widgets). 

#### `ImGuiStyle.FrameRounding`

`attribute float ImGuiStyle.FrameRounding`

Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets). 

#### `ImGuiStyle.FrameBorderSize`

`attribute float ImGuiStyle.FrameBorderSize`

Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly). 

#### `ImGuiStyle.ItemSpacing`

`attribute ImVec2* ImGuiStyle.ItemSpacing`

Horizontal and vertical spacing between widgets/lines. 

#### `ImGuiStyle.ItemInnerSpacing`

`attribute ImVec2* ImGuiStyle.ItemInnerSpacing`

Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label). 

#### `ImGuiStyle.TouchExtraPadding`

`attribute ImVec2* ImGuiStyle.TouchExtraPadding`

Expand reactive bounding box for touch-based system where touch position is not accurate enough. 
Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. 
Don't grow this too much! 

#### `ImGuiStyle.IndentSpacing`

`attribute float ImGuiStyle.IndentSpacing`

Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2). 

#### `ImGuiStyle.ColumnsMinSpacing`

`attribute float ImGuiStyle.ColumnsMinSpacing`

Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1). 

#### `ImGuiStyle.ScrollbarSize`

`attribute float ImGuiStyle.ScrollbarSize`

Width of the vertical scrollbar, Height of the horizontal scrollbar. 

#### `ImGuiStyle.ScrollbarRounding`

`attribute float ImGuiStyle.ScrollbarRounding`

Radius of grab corners for scrollbar. 

#### `ImGuiStyle.GrabMinSize`

`attribute float ImGuiStyle.GrabMinSize`

Minimum width/height of a grab box for slider/scrollbar. 

#### `ImGuiStyle.GrabRounding`

`attribute float ImGuiStyle.GrabRounding`

Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs. 

#### `ImGuiStyle.TabRounding`

`attribute float ImGuiStyle.TabRounding`

Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs. 

#### `ImGuiStyle.TabBorderSize`

`attribute float ImGuiStyle.TabBorderSize`

Thickness of border around tabs. 

#### `ImGuiStyle.TabMinWidthForUnselectedCloseButton`

`attribute float ImGuiStyle.TabMinWidthForUnselectedCloseButton`

Minimum width for close button to appears on an unselected tab when hovered. 
Set to 0.0f to always show when hovering, set to FLT_MAX to never show close button unless selected. 

#### `ImGuiStyle.ColorButtonPosition`

`attribute ImGuiDir ImGuiStyle.ColorButtonPosition`

Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir_Right. 

#### `ImGuiStyle.ButtonTextAlign`

`attribute ImVec2* ImGuiStyle.ButtonTextAlign`

Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered). 

#### `ImGuiStyle.SelectableTextAlign`

`attribute ImVec2* ImGuiStyle.SelectableTextAlign`

Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). 
It's generally important to keep this left-aligned if you want to lay multiple items on a same line. 

#### `ImGuiStyle.DisplayWindowPadding`

`attribute ImVec2* ImGuiStyle.DisplayWindowPadding`

Window position are clamped to be visible within the display area by at least this amount. 
Only applies to regular windows. 

#### `ImGuiStyle.DisplaySafeAreaPadding`

`attribute ImVec2* ImGuiStyle.DisplaySafeAreaPadding`

If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. 
Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly! 

#### `ImGuiStyle.MouseCursorScale`

`attribute float ImGuiStyle.MouseCursorScale`

Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later. 

#### `ImGuiStyle.AntiAliasedLines`

`attribute bool ImGuiStyle.AntiAliasedLines`

Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU. 

#### `ImGuiStyle.AntiAliasedFill`

`attribute bool ImGuiStyle.AntiAliasedFill`

Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.) 

#### `ImGuiStyle.CurveTessellationTol`

`attribute float ImGuiStyle.CurveTessellationTol`

Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. 
Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality. 

#### `ImGuiStyle.CircleSegmentMaxError`

`attribute float ImGuiStyle.CircleSegmentMaxError`

Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles 
with no explicit segment count specified. Decrease for higher quality but more geometry. 

#### `ImGuiStyle.Colors`

`attribute ImVec4* ImGuiStyle.Colors[]`

Use to configure the colors used in varied parts of the ImGui theme. Check the [`enum ImGuiCol_`](#enum-imguicol_) for
all possible colors available in a ImGuiStyle object. An example usage would be:
                                                      
```AGS Script
ImGuiStyle* style = ImGui.GetStyle();
style.Colors[eImGuiCol_TextDisabled] = ImVec4.Create(0.5, 0.5, 0.5, 0.5);
ImGui.SetStyle(style);
```


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

- `eImGuiBeginWindow_OK_Closed` the window is closed. This value is only returned at mouse release frame, after clicking on close button!

- `eImGuiBeginWindow_Collapsed_Closed` the window is both collapsed and closed, rare but theoretically possible.

#### `ImGuiWindowFlags`

- `eImGuiWindowFlags_None` (= 0),

- `eImGuiWindowFlags_NoTitleBar`, Disable title-bar

- `eImGuiWindowFlags_NoResize`, Disable user resizing with the lower-right grip

- `eImGuiWindowFlags_NoMove`,  Disable user moving the window

- `eImGuiWindowFlags_NoScrollbar`, Disable scrollbars (window can still scroll with mouse or programmatically)

- `eImGuiWindowFlags_NoScrollWithMouse`, Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.

- `eImGuiWindowFlags_NoCollapse`, Disable user collapsing window by double-clicking on it

- `eImGuiWindowFlags_AlwaysAutoResize`, Resize every window to its content every frame

- `eImGuiWindowFlags_NoBackground`,Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).

- `eImGuiWindowFlags_NoSavedSettings`, Never load/save settings in .ini file

- `eImGuiWindowFlags_NoMouseInputs`, Disable catching mouse, hovering test with pass through.

- `eImGuiWindowFlags_MenuBar`, Has a menu-bar

- `eImGuiWindowFlags_HorizontalScrollbar`, Allow horizontal scrollbar to appear (off by default).

- `eImGuiWindowFlags_NoFocusOnAppearing`, Disable taking focus when transitioning from hidden to visible state

- `eImGuiWindowFlags_NoBringToFrontOnFocus`, Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)

- `eImGuiWindowFlags_AlwaysVerticalScrollbar`, Always show vertical scrollbar (even if ContentSize.y < Size.y)

- `eImGuiWindowFlags_AlwaysHorizontalScrollbar`, Always show horizontal scrollbar (even if ContentSize.x < Size.x)

- `eImGuiWindowFlags_AlwaysUseWindowPadding`, Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)

- `eImGuiWindowFlags_NoNavInputs`, No gamepad/keyboard navigation within the window

- `eImGuiWindowFlags_NoNavFocus`, No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)

- `eImGuiWindowFlags_UnsavedDocument`, Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator.

- `eImGuiWindowFlags_NoNav`,

- `eImGuiWindowFlags_NoDecoration`,

- `eImGuiWindowFlags_NoInputs`,

#### `enum ImGuiInputTextFlags`

The enums of this type can be combined with bitwise operators. 

- `eImGuiInputTextFlags_None`, (=0) default

- `eImGuiInputTextFlags_CharsDecimal`, Allow 0123456789.+-*/

- `eImGuiInputTextFlags_CharsHexadecimal`, Allow 0123456789ABCDEFabcdef

- `eImGuiInputTextFlags_CharsUppercase`, Turn a..z into A..Z

- `eImGuiInputTextFlags_CharsNoBlank`, Filter out spaces, tabs

- `eImGuiInputTextFlags_AutoSelectAll`, Select entire text when first taking mouse focus

- `eImGuiInputTextFlags_EnterReturnsTrue`, Return 'true' when Enter is pressed (as opposed to every time the value was modified).

- `eImGuiInputTextFlags_CallbackCompletion`, Callback on pressing TAB (for completion handling)

- `eImGuiInputTextFlags_CallbackHistory`, Callback on pressing Up/Down arrows (for history handling)

- `eImGuiInputTextFlags_CallbackAlways`, Callback on each iteration. User code may query cursor position, modify text buffer.

- `eImGuiInputTextFlags_CallbackCharFilter`, Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.

- `eImGuiInputTextFlags_AllowTabInput`, Pressing TAB input a 't' character into the text field

- `eImGuiInputTextFlags_CtrlEnterForNewLine`, In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).

- `eImGuiInputTextFlags_NoHorizontalScroll`, Disable following the cursor horizontally

- `eImGuiInputTextFlags_AlwaysInsertMode`, Insert mode

- `eImGuiInputTextFlags_ReadOnly`, Read-only mode

- `eImGuiInputTextFlags_Password`, Password mode, display all characters as '*'

- `eImGuiInputTextFlags_NoUndoRedo`, Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().

- `eImGuiInputTextFlags_CharsScientific`, Allow 0123456789.+-*/eE (Scientific notation input)

- `eImGuiInputTextFlags_CallbackResize`, Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow.

#### `enum ImGuiTreeNodeFlags`

Flags for AgsImGui.TreeNodeEx(), AgsImGui.CollapsingHeader().

-`eImGuiTreeNodeFlags_None`, (=0) default

-`eImGuiTreeNodeFlags_Selected`, Draw as selected

-`eImGuiTreeNodeFlags_Framed`, Full colored frame (e.g. for CollapsingHeader)

-`eImGuiTreeNodeFlags_AllowItemOverlap`, Hit testing to allow subsequent widgets to overlap this one

-`eImGuiTreeNodeFlags_NoTreePushOnOpen`, Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack

-`eImGuiTreeNodeFlags_NoAutoOpenOnLog`, Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)

-`eImGuiTreeNodeFlags_DefaultOpen`, Default node to be open

-`eImGuiTreeNodeFlags_OpenOnDoubleClick`, Need double-click to open node

-`eImGuiTreeNodeFlags_OpenOnArrow`, Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.

-`eImGuiTreeNodeFlags_Leaf`, No collapsing, no arrow (use as a convenience for leaf nodes).

-`eImGuiTreeNodeFlags_Bullet`, Display a bullet instead of arrow

-`eImGuiTreeNodeFlags_FramePadding`, Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().

-`eImGuiTreeNodeFlags_SpanAvailWidth`, Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.

-`eImGuiTreeNodeFlags_SpanFullWidth`, Extend hit box to the left-most and right-most edges (bypass the indented area).

-`eImGuiTreeNodeFlags_NavLeftJumpsBackHere`, (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)

-`eImGuiTreeNodeFlags_NoScrollOnOpen`, FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible

-`eImGuiTreeNodeFlags_CollapsingHeader`, Framed, NoTreePushOpen and NoAutoOpenOnLog.


#### `enum ImGuiCol_`

Used for selecting a specific color from the `ImGuiStyle.Colors[]` property.

An example usage would be:

```AGS Script
ImGuiStyle* style = ImGui.GetStyle();
style.Colors[eImGuiCol_WindowBg] = ImVec4.Create(1.0, 0.0, 0.0, 0.0);
ImGui.SetStyle(style);
```

This would use the enum `eImGuiCol_WindowBg` to access the desired color you want to modify, 
which in the example sets to a red color.

- `eImGuiCol_Text`, 

- `eImGuiCol_TextDisabled`, 

- `eImGuiCol_WindowBg`, Background of normal windows 

- `eImGuiCol_ChildBg`, Background of child windows 

- `eImGuiCol_PopupBg`, Background of popups, menus, tooltips windows 

- `eImGuiCol_Border`,

- `eImGuiCol_BorderShadow`,

- `eImGuiCol_FrameBg`, Background of checkbox, radio button, plot, slider, text input

- `eImGuiCol_FrameBgHovered`,

- `eImGuiCol_FrameBgActive`, 

- `eImGuiCol_TitleBg`,

- `eImGuiCol_TitleBgActive`,

- `eImGuiCol_TitleBgCollapsed`,

- `eImGuiCol_MenuBarBg`,

- `eImGuiCol_ScrollbarBg`,

- `eImGuiCol_ScrollbarGrab`,

- `eImGuiCol_ScrollbarGrabHovered`,

- `eImGuiCol_ScrollbarGrabActive`,

- `eImGuiCol_CheckMark`,

- `eImGuiCol_SliderGrab`,

- `eImGuiCol_SliderGrabActive`,

- `eImGuiCol_Button`,

- `eImGuiCol_ButtonHovered`, 

- `eImGuiCol_ButtonActive`,

- `eImGuiCol_Header`, Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem

- `eImGuiCol_HeaderHovered`,

- `eImGuiCol_HeaderActive`,

- `eImGuiCol_Separator`,

- `eImGuiCol_SeparatorHovered`,

- `eImGuiCol_SeparatorActive`,

- `eImGuiCol_ResizeGrip`,

- `eImGuiCol_ResizeGripHovered`,

- `eImGuiCol_ResizeGripActive`,

- `eImGuiCol_Tab`,

- `eImGuiCol_TabHovered`, 

- `eImGuiCol_TabActive`,

- `eImGuiCol_TabUnfocused`, 

- `eImGuiCol_TabUnfocusedActive`, 

- `eImGuiCol_PlotLines`,

- `eImGuiCol_PlotLinesHovered`,

- `eImGuiCol_PlotHistogram`,

- `eImGuiCol_PlotHistogramHovered`,

- `eImGuiCol_TextSelectedBg`,

- `eImGuiCol_DragDropTarget`, 

- `eImGuiCol_NavHighlight`, Gamepad/keyboard: current highlighted item;

- `eImGuiCol_NavWindowingHighlight`, Highlight window when using CTRL+TAB;

- `eImGuiCol_NavWindowingDimBg`, Darken/colorize entire screen behind the CTRL+TAB window list, when active;

- `eImGuiCol_ModalWindowDimBg`, Darken/colorize entire screen behind a modal window, when one is active;

- `eImGuiCol_COUNT`, The total numbers of items in ImGuiStyle.Colors attribute array. Useful for iterating all colors. 

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

