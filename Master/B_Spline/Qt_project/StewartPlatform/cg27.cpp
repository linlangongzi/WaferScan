#include "CG27.h"

//extern DIJOYSTATE2 g_G27State;
//extern QMutex g_mutexG27;
DIJOYSTATE2 g_G27State;
QMutex g_mutexG27;
double lr_angle;


LPDIRECTINPUT8          g_pDI = NULL;
LPDIRECTINPUTDEVICE8    g_pJoystick = NULL;


CG27::CG27(void)
{
    //  InitializeG27();
    InitializeG27();
    qDebug() << "initializing";
    //run();
}

CG27::~CG27(void)
{
}


//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK
enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice(instance->guidInstance, &g_pJoystick, NULL);

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if (FAILED(hr)) {
        return DIENUM_CONTINUE;
    }

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    //0‡20†70‡5¡ã0‰00†30†70†40‡00‡50ˆ60†30†00‹2¡¤0†50ˆ3¨°0‡30ˆ0
    return DIENUM_STOP;
}


BOOL CALLBACK
enumAxesCallback(const DIDEVICEOBJECTINSTANCE* instance, VOID* context)
{
    HWND hDlg = (HWND)context;

    DIPROPRANGE propRange;
    propRange.diph.dwSize       = sizeof(DIPROPRANGE);
    propRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    propRange.diph.dwHow        = DIPH_BYID;
    propRange.diph.dwObj        = instance->dwType;
    if (instance->guidType == GUID_XAxis )//¡¤0†50ˆ3¨°0‡30ˆ0
    {
        propRange.lMin              = -4500;
        propRange.lMax              = +4500;
    }
    else if(instance->guidType == GUID_Slider )//0†80Š50†20ˆ3
    {
        propRange.lMin              = 0;
        propRange.lMax              = 1000;
    }
    else if(instance->guidType == GUID_RzAxis )//0‡70…50…60…8
    {
        propRange.lMin              = 0;
        propRange.lMax              = +1000;
    }
    else if(instance->guidType == GUID_YAxis )//0ˆ70ˆ10‡10‡3
    {
        propRange.lMin              = 0;
        propRange.lMax              = +1000;
    }
    else
    {
        propRange.lMin              = -1000;
        propRange.lMax              = +1000;
    }



    // Set the range for the axis
    if (FAILED(g_pJoystick->SetProperty(DIPROP_RANGE, &propRange.diph))) {
        return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}

bool CG27::InitializeG27()
{
    HRESULT hr;

    // Register with the DirectInput subsystem and get a pointer to a IDirectInput interface we can use.
    // Create a DInput object

    //initialize directinput library
    if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
        IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) )
        return false;


    LPDIRECTINPUTDEVICE8 joystick;

    // Look for the first simple joystick we can find.
    if (FAILED(hr = g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback,
        NULL, DIEDFL_ATTACHEDONLY))) {
            return false;
    }

    if (g_pJoystick == NULL)
    {
//        ShowMessageBox("g27 not found, please check the connection, exiting........");
        qDebug() << "fucklrl";
        return false;
    }

    if (FAILED(hr = g_pJoystick->SetDataFormat(&c_dfDIJoystick2)))
    {
//        ShowMessageBox(" set g27 data format error, exiting.......");
        qDebug() << "fuckld";
        return false;
    }

    g_pJoystick->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE|DISCL_FOREGROUND);

    DIDEVCAPS capabilities;
    capabilities.dwSize = sizeof(DIDEVCAPS);

    g_pJoystick->GetCapabilities(&capabilities);

    if (FAILED(hr=g_pJoystick->EnumObjects(enumAxesCallback, NULL, DIDFT_AXIS)))
    {

    }
    qDebug() << "initializing succeeded";
    return true;
}

QVector<qreal> CG27::ReadData()
{
    QVector<double> joints(3);
    joints[0] = g_G27State.lX;
    joints[1] = g_G27State.lY;
    joints[2] = g_G27State.lRz;
    return joints;
}

void CG27::run()
{
    HRESULT hr;
    if (g_pJoystick == NULL)
    {
        return;
    }


    while(1)
    {
        hr = g_pJoystick->Poll();
        g_pJoystick->Acquire();
        g_mutexG27.lock();
        hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &g_G27State);
        emit(GetG27Info());

//        qDebug() << "FangXiangPan :" << g_G27State.lX;	//¡¤0†50ˆ3¨°0‡30ˆ00„5¡§-4500,45000„50„8
//        qDebug() << "Power: " << g_G27State.lY;
//        qDebug() << g_G27State.lZ;
//        qDebug() << g_G27State.lRx;
//        qDebug() << g_G27State.lRy;
//        qDebug() << "Brade: " << g_G27State.lRz;
//        qDebug() << g_G27State.lVX;
//        qDebug() << g_G27State.lVY;
//        qDebug() << g_G27State.lVZ;
//        qDebug() << g_G27State.lAX;
//        qDebug() << g_G27State.lAY;
//        qDebug() << g_G27State.lAZ;
//        qDebug() << " ";

        lr_angle = g_G27State.lX;
        //w.sixJointsForSteerPlatform.SetPos(0,0,400,0,lr_angle,0);
        g_mutexG27.unlock();
//        QVector<double> joints(3);
//        joints[0] = g_G27State.lX;
//        joints[1] = g_G27State.lY;
//        joints[2] = g_G27State.lRz;
        Sleep(25);
    }

    return;

}
