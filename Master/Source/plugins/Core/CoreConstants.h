/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include <QtGlobal>

namespace Core {
namespace Constants {

// Modes
const char MODE_WELCOME[]          = "Welcome";
const char MODE_EDIT[]             = "Edit";
const char MODE_DESIGN[]           = "Design";
const int  P_MODE_WELCOME          = 100;
const int  P_MODE_EDIT             = 90;
const int  P_MODE_DESIGN           = 89;

// Menubar
const char MENU_BAR[]              = "WaferScan.MenuBar";

// Menus
const char M_FILE[]                = "WaferScan.Menu.File";
const char M_VIEW[]                = "WaferScan.Menu.View";
const char M_SCAN[]                = "WaferScan.Menu.Scan";
const char M_WINDOW[]              = "WaferScan.Menu.Window";
const char M_WINDOW_PANES[]        = "WaferScan.Menu.Window.Panes";
const char M_WINDOW_VIEWS[]        = "WaferScan.Menu.Window.Views";
const char M_HELP[]                = "WaferScan.Menu.Help";

// Contexts
const char C_GLOBAL[]              = "Global Context";
const char C_WELCOME_MODE[]        = "Core.WelcomeMode";
const char C_EDIT_MODE[]           = "Core.EditMode";
const char C_DESIGN_MODE[]         = "Core.DesignMode";
const char C_EDITORMANAGER[]       = "Core.EditorManager";
const char C_NAVIGATION_PANE[]     = "Core.NavigationPane";
const char C_PROBLEM_PANE[]        = "Core.ProblemPane";
const char C_GENERAL_OUTPUT_PANE[] = "Core.GeneralOutputPane";

// Default editor kind
const char K_DEFAULT_TEXT_EDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "Plain Text Editor");
const char K_DEFAULT_TEXT_EDITOR_ID[] = "Core.PlainTextEditor";
const char K_DEFAULT_BINARY_EDITOR_ID[] = "Core.BinaryEditor";

//actions
const char UNDO[]                  = "WaferScan.Undo";
const char REDO[]                  = "WaferScan.Redo";
const char COPY[]                  = "WaferScan.Copy";
const char PASTE[]                 = "WaferScan.Paste";
const char CUT[]                   = "WaferScan.Cut";
const char SELECTALL[]             = "WaferScan.SelectAll";

const char GOTO[]                  = "WaferScan.Goto";

const char NEW[]                   = "WaferScan.New";
const char OPEN[]                  = "WaferScan.Open";
const char RELOAD_FILE[]           = "WaferScan.ReloadFile";
const char OPEN_WITH[]             = "WaferScan.OpenWith";
const char IMPORT[]                = "WaferScan.Import";
const char REVERTTOSAVED[]         = "WaferScan.RevertToSaved";
const char SAVE[]                  = "WaferScan.Save";
const char SAVEAS[]                = "WaferScan.SaveAs";
const char SAVEALL[]               = "WaferScan.SaveAll";
const char PRINT[]                 = "WaferScan.Print";
const char EXIT[]                  = "WaferScan.Exit";

const char OPTIONS[]               = "WaferScan.Options";
const char CONNECTIONS[]           = "WaferScan.Connections";
const char MATERIAL[]              = "WaferScan.Material";
const char PROBE[]                 = "WaferScan.Probe";
const char TASK_INFO[]             = "WaferScan.TaskInfo";

const char IMPORT_LAYOUT[]         = "WaferScan.ImportLayout";
const char EXPORT_LAYOUT[]         = "WaferScan.ExportLayout";
const char RESET_LAYOUT[]          = "WaferScan.ResetLayout";
const char IMPORT_WORKSPACE[]      = "WaferScan.ImportWorkspace";
const char EXPORT_WORKSPACE[]      = "WaferScan.ExportWorkspace";

const char TOGGLE_SIDEBAR[]        = "WaferScan.ToggleSidebar";
const char TOGGLE_MODE_SELECTOR[]  = "WaferScan.ToggleModeSelector";
const char TOGGLE_FULLSCREEN[]     = "WaferScan.ToggleFullScreen";
const char THEMEOPTIONS[]          = "WaferScan.ThemeOptions";

const char TR_SHOW_SIDEBAR[]       = QT_TRANSLATE_NOOP("Core", "Show Sidebar");
const char TR_HIDE_SIDEBAR[]       = QT_TRANSLATE_NOOP("Core", "Hide Sidebar");

const char MINIMIZE_WINDOW[]       = "WaferScan.MinimizeWindow";
const char ZOOM_WINDOW[]           = "WaferScan.ZoomWindow";
const char CLOSE_WINDOW[]           = "WaferScan.CloseWindow";

const char SPLIT[]                 = "WaferScan.Split";
const char SPLIT_SIDE_BY_SIDE[]    = "WaferScan.SplitSideBySide";
const char SPLIT_NEW_WINDOW[]      = "WaferScan.SplitNewWindow";
const char REMOVE_CURRENT_SPLIT[]  = "WaferScan.RemoveCurrentSplit";
const char REMOVE_ALL_SPLITS[]     = "WaferScan.RemoveAllSplits";
const char GOTO_PREV_SPLIT[]       = "WaferScan.GoToPreviousSplit";
const char GOTO_NEXT_SPLIT[]       = "WaferScan.GoToNextSplit";
const char CLOSE[]                 = "WaferScan.Close";
const char CLOSE_ALTERNATIVE[]     = "WaferScan.Close_Alternative"; // temporary, see WaferScanBUG-72
const char CLOSEALL[]              = "WaferScan.CloseAll";
const char CLOSEOTHERS[]           = "WaferScan.CloseOthers";
const char CLOSEALLEXCEPTVISIBLE[] = "WaferScan.CloseAllExceptVisible";
const char GOTONEXT[]              = "WaferScan.GotoNext";
const char GOTOPREV[]              = "WaferScan.GotoPrevious";
const char GOTONEXTINHISTORY[]     = "WaferScan.GotoNextInHistory";
const char GOTOPREVINHISTORY[]     = "WaferScan.GotoPreviousInHistory";
const char GO_BACK[]               = "WaferScan.GoBack";
const char GO_FORWARD[]            = "WaferScan.GoForward";
const char BROWSE_CONFIG[]         = "WaferScan.BrowseConfig";
const char ABOUT_WAFERSCAN[]       = "WaferScan.AboutWaferScan";
const char ABOUT_PLUGINS[]         = "WaferScan.AboutPlugins";
const char S_RETURNTOEDITOR[]      = "WaferScan.ReturnToEditor";

// Default groups
const char G_DEFAULT_ONE[]         = "WaferScan.Group.Default.One";
const char G_DEFAULT_TWO[]         = "WaferScan.Group.Default.Two";
const char G_DEFAULT_THREE[]       = "WaferScan.Group.Default.Three";

// Main menu bar groups
const char G_DOCUMENT[]            = "WaferScan.Group.Document";
const char G_FILE[]                = "WaferScan.Group.File";
const char G_EDIT[]                = "WaferScan.Group.Edit";
const char G_VIEW[]                = "WaferScan.Group.View";
const char G_TOOLS[]               = "WaferScan.Group.Tools";
const char G_WINDOW[]              = "WaferScan.Group.Window";
const char G_HELP[]                = "WaferScan.Group.Help";

// File menu groups
const char G_FILE_NEW[]            = "WaferScan.Group.File.New";
const char G_FILE_OPEN[]           = "WaferScan.Group.File.Open";
const char G_FILE_PROJECT[]        = "WaferScan.Group.File.Project";
const char G_FILE_SAVE[]           = "WaferScan.Group.File.Save";
const char G_FILE_CLOSE[]          = "WaferScan.Group.File.Close";
const char G_FILE_PRINT[]          = "WaferScan.Group.File.Print";
const char G_FILE_OPTION[]         = "WaferScan.Group.File.Option";
const char G_FILE_OTHER[]          = "WaferScan.Group.File.Other";

// View menu groups
const char G_VIEW_PANEL[]          = "WaferScan.Group.View.Panel";
const char G_VIEW_LAYOUT[]         = "WaferScan.Group.View.Layout";
const char G_VIEW_WORKSPACE[]      = "WaferScan.Group.View.Workspace";

const char G_TOOLS_OPTIONS[]       = "WaferScan.Group.Tools.Options";

// Window menu groups
const char G_WINDOW_SIZE[]         = "WaferScan.Group.Window.Size";
const char G_WINDOW_PANES[]        = "WaferScan.Group.Window.Panes";
const char G_WINDOW_VIEWS[]        = "WaferScan.Group.Window.Views";
const char G_WINDOW_SPLIT[]        = "WaferScan.Group.Window.Split";
const char G_WINDOW_NAVIGATE[]     = "WaferScan.Group.Window.Navigate";
const char G_WINDOW_LIST[]         = "WaferScan.Group.Window.List";
const char G_WINDOW_OTHER[]        = "WaferScan.Group.Window.Other";

// Help groups (global)
const char G_HELP_HELP[]           = "WaferScan.Group.Help.Help";
const char G_HELP_SUPPORT[]        = "WaferScan.Group.Help.Supprt";
const char G_HELP_ABOUT[]          = "WaferScan.Group.Help.About";
const char G_HELP_UPDATES[]        = "WaferScan.Group.Help.Updates";

const char WIZARD_CATEGORY_QT[] = "R.Qt";
const char WIZARD_TR_CATEGORY_QT[] = QT_TRANSLATE_NOOP("Core", "Qt");
const char WIZARD_KIND_UNKNOWN[] = "unknown";
const char WIZARD_KIND_PROJECT[] = "project";
const char WIZARD_KIND_FILE[] = "file";

const char SETTINGS_CATEGORY_CORE[] = "A.Core";
const char SETTINGS_CATEGORY_CORE_ICON[] = ":/core/images/category_core.png";
const char SETTINGS_TR_CATEGORY_CORE[] = QT_TRANSLATE_NOOP("Core", "Environment");
const char SETTINGS_ID_GENERAL[] = "A.General";
//const char SETTINGS_ID_SYSTEM[] = "B.Core.System";
const char SETTINGS_ID_SHORTCUTS[] = "C.Keyboard";
//const char SETTINGS_ID_TOOLS[] = "D.ExternalTools";
//const char SETTINGS_ID_MIMETYPES[] = "E.MimeTypes";

const char SETTINGS_DEFAULTTEXTENCODING[] = "General/DefaultFileEncoding";

const char SETTINGS_THEME[] = "Core/CreatorTheme";
const char DEFAULT_THEME[] = "flat";

const char TR_CLEAR_MENU[]         = QT_TRANSLATE_NOOP("Core", "Clear Menu");

const char DEFAULT_BUILD_DIRECTORY[] = "../build-%{CurrentProject:Name}-%{CurrentKit:FileSystemName}-%{CurrentBuild:Name}";

const int TARGET_ICON_SIZE = 32;

const char ARS_FILE_EXT_DATA[] = ".ard";
const char ARS_FILE_EXT_PARAMETER[] = ".arp";

} // namespace Constants
} // namespace Core
