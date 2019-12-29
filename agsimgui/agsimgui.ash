// ags imgui module header

enum ImGuiDir
{
  eImGuiDir_None    = -1,
  eImGuiDir_Left    = 0,
  eImGuiDir_Right   = 1,
  eImGuiDir_Up      = 2,
  eImGuiDir_Down    = 3,
  eImGuiDir_COUNT
};

enum ImGuiWindowFlags
{
  eImGuiWindowFlags_None                   = 0,
  eImGuiWindowFlags_NoTitleBar             = 1,   // Disable title-bar
  eImGuiWindowFlags_NoResize               = 2,   // Disable user resizing with the lower-right grip
  eImGuiWindowFlags_NoMove                 = 4,   // Disable user moving the window
  eImGuiWindowFlags_NoScrollbar            = 8,   // Disable scrollbars (window can still scroll with mouse or programmatically)
  eImGuiWindowFlags_NoScrollWithMouse      = 16,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
  eImGuiWindowFlags_NoCollapse             = 32,   // Disable user collapsing window by double-clicking on it
  eImGuiWindowFlags_AlwaysAutoResize       = 64,   // Resize every window to its content every frame
  eImGuiWindowFlags_NoBackground           = 128,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
  eImGuiWindowFlags_NoSavedSettings        = 256,   // Never load/save settings in .ini file
  eImGuiWindowFlags_NoMouseInputs          = 512,   // Disable catching mouse, hovering test with pass through.
  eImGuiWindowFlags_MenuBar                = 1024,  // Has a menu-bar
  eImGuiWindowFlags_HorizontalScrollbar    = 2048,  // Allow horizontal scrollbar to appear (off by default).
  eImGuiWindowFlags_NoFocusOnAppearing     = 4096,  // Disable taking focus when transitioning from hidden to visible state
  eImGuiWindowFlags_NoBringToFrontOnFocus  = 8192,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
  eImGuiWindowFlags_AlwaysVerticalScrollbar= 16384,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
  eImGuiWindowFlags_AlwaysHorizontalScrollbar= 32768,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
  eImGuiWindowFlags_AlwaysUseWindowPadding = 65536,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
  eImGuiWindowFlags_NoNavInputs            = 262144,  // No gamepad/keyboard navigation within the window
  eImGuiWindowFlags_NoNavFocus             = 524288,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
  eImGuiWindowFlags_UnsavedDocument        = 1048576,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator.
  eImGuiWindowFlags_NoNav                  = 786432,
  eImGuiWindowFlags_NoDecoration           = 43,
  eImGuiWindowFlags_NoInputs               = 786944,
};

enum ImGuiInputTextFlags
{
  eImGuiInputTextFlags_None                = 0,
  eImGuiInputTextFlags_CharsDecimal        = 1,   // Allow 0123456789.+-*/
  eImGuiInputTextFlags_CharsHexadecimal    = 2,   // Allow 0123456789ABCDEFabcdef
  eImGuiInputTextFlags_CharsUppercase      = 4,   // Turn a..z into A..Z
  eImGuiInputTextFlags_CharsNoBlank        = 8,   // Filter out spaces, tabs
  eImGuiInputTextFlags_AutoSelectAll       = 16,   // Select entire text when first taking mouse focus
  eImGuiInputTextFlags_EnterReturnsTrue    = 32,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified).
  eImGuiInputTextFlags_CallbackCompletion  = 64,   // Callback on pressing TAB (for completion handling)
  eImGuiInputTextFlags_CallbackHistory     = 128,   // Callback on pressing Up/Down arrows (for history handling)
  eImGuiInputTextFlags_CallbackAlways      = 256,   // Callback on each iteration. User code may query cursor position, modify text buffer.
  eImGuiInputTextFlags_CallbackCharFilter  = 512,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
  eImGuiInputTextFlags_AllowTabInput       = 1024,  // Pressing TAB input a '\t' character into the text field
  eImGuiInputTextFlags_CtrlEnterForNewLine = 2048,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
  eImGuiInputTextFlags_NoHorizontalScroll  = 4096,  // Disable following the cursor horizontally
  eImGuiInputTextFlags_AlwaysInsertMode    = 8192,  // Insert mode
  eImGuiInputTextFlags_ReadOnly            = 16384,  // Read-only mode
  eImGuiInputTextFlags_Password            = 32768,  // Password mode, display all characters as '*'
  eImGuiInputTextFlags_NoUndoRedo          = 65536,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
  eImGuiInputTextFlags_CharsScientific     = 131072,  // Allow 0123456789.+-*/eE (Scientific notation input)
  eImGuiInputTextFlags_CallbackResize      = 262144,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow.
};

struct agsimgui{
  // Main

  /// start a new Dear ImGui frame, you can submit any command from this point until Render()/EndFrame().
  import static void NewFrame();                                 

  /// ends the Dear ImGui frame. automatically called by Render(), you likely don't need to call that yourself directly. 
  import static void EndFrame();

  /// ends the Dear ImGui frame, finalize the draw data. You can get call GetDrawData() to obtain it and run your rendering function. 
  import static void Render();                                   

  /// valid after Render() and until the next call to NewFrame(). this is what you have to render.
  import static int GetDrawData();


  // Demo, Debug, Information

  /// get the compiled version string e.g. "1.23" (essentially the compiled value for IMGUI_VERSION)
  import static String GetVersion();


  // Windows

  /// Push window to the stack. Always call a matching End() for each Begin() call. Return false to indicate the window is collapsed or fully clipped
  import static bool Begin(String name, ImGuiWindowFlags flags = 0);

  /// pop window from the stack.
  import static void End();

  // Child Windows

  /// Child Windows. Always call a matching EndChild() for each BeginChild() call, regardless of its return value. Child windows can embed their own child.
  import static bool BeginChild(String str_id, int width = 0, int height = 0, bool border = false, ImGuiWindowFlags flags = 0);

  /// pop child window from the stack.
  import static void EndChild();


  // Widgets: Text

  /// draws text
  import static void          Text(String text);

  /// shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
  import static void          TextColored(ColorType color, String text);

  /// shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
  import static void          TextDisabled(String text);

  /// shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-
  import static void          TextWrapped(String text);

  /// display text+label aligned the same way as value+label widgets
  import static void          LabelText(String label, String text);

  /// shortcut for Bullet()+Text()
  import static void          BulletText(String text);


  // Widgets: Main
  // - Most widgets return true when the value has been changed or when pressed/selected
  // - You may also use one of the many IsItemXXX functions (e.g. IsItemActive, IsItemHovered, etc.) to query widget state.

  /// button
  import static bool Button(String label, int width, int height);

  /// button with FramePadding=(0,0) to easily embed within text
  import static bool SmallButton(String label); 

  /// square button with an arrow shape
  import static bool ArrowButton(String str_id, ImGuiDir dir);                  

  import static bool Checkbox(String label, bool v);

  /// use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
  import static bool RadioButton(String label, bool active);                            

  /// draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses
  import static  void Bullet();                                                        
};