#pragma once

namespace Motion {

static const char SETTINGS_CATEGORY_MOTION[] = "C.Motion";
static const char SETTINGS_ICON_MOTION[] = ":/fatcow/16x16/steering_wheel_3.png";
static const char SETTINGS_ID_MOTION[] = "A.Motion";

static const char F_AXIS[] = "Axis.json";
static const char F_ENDPOINT[] = "EndPoint.json";
static const char F_GLOBAL_POSITION[] = "GlobalPosition.json";
static const char K_CONTROLLER_HOST[] = "ControllerHost";
static const char K_CONTROLLER_IO_CONFIG[] = "IoConfig";
static const char K_CONTROLLER_SAFE_MODULE[] = "SafeGuard";
static const char K_CONTROLLER_SIMULATE_DYNAMIC[] = "SimulateDynamics";
static const char K_AXIS_PRESITION[] = "AxisPresition";
// Main menu bar
static const char G_MOTION[] = "WaferScan.Group.Motion";
static const char G_MOTION_SETTINGS[] = "Motion";
// Motion menu groups
static const char G_CONNECTION[] = "WaferScan.Group.Motion.Connection";
static const char G_MANUAL_CONTROL[] = "WaferScan.Group.Motion.ManualControl";
static const char G_MOTION_MODE[] = "WaferScan.Group.Motion.Mode";

static const char CONNECT[] = "Motion.Connect";
static const char DISCONNECT[] = "Motion.Disconnect";

static const char SAFETY_BARRIER[] = "Motion.SafetyBarrier";
static const char ENFORCE_STEP[] = "Motion.EnforceStep";
static const char HOTKEY_ENABLE[] = "Motion.HotkeyEnable";

static const char MANUAL_MODE[] = "Motion.ManualMode";
static const char REMOTE_MODE[] = "Motion.RemoteMode";
static const char CIRCULAR_MODE[] = "Motion.CircularMode";
static const char TCP_MODE[] = "Motion.TcpMode";
}
