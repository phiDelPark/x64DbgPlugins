#include "pluginmain.h"

#define plugin_name "Easy Label View"
#define plugin_version 1
#define plugin_author "phiDel (is4975@gmail.com)"

int Plugin::handle;
HWND Plugin::hwndDlg;
int Plugin::hMenu;
int Plugin::hMenuDisasm;
int Plugin::hMenuDump;
int Plugin::hMenuStack;

bool cmdSetBPX(duint addr, bool set)
{
    char sAddr[50];
    sprintf_s(sAddr, (set ? "SetBPX %p" : "DeleteBPX %p"), addr);
    return DbgCmdExecDirect(sAddr);
}

extern "C" __declspec(dllexport) bool pluginit(PLUG_INITSTRUCT* initStruct)
{
    initStruct->pluginVersion = plugin_version;
    initStruct->sdkVersion = PLUG_SDKVERSION;
    strcpy_s(initStruct->pluginName, plugin_name);

    Plugin::handle = initStruct->pluginHandle;
    QtPlugin::Init();
    return true;
}

extern "C" __declspec(dllexport) void plugsetup(PLUG_SETUPSTRUCT* setupStruct)
{
    Plugin::hwndDlg = setupStruct->hwndDlg;
    Plugin::hMenu = setupStruct->hMenu;
    Plugin::hMenuDisasm = setupStruct->hMenuDisasm;
    Plugin::hMenuDump = setupStruct->hMenuDump;
    Plugin::hMenuStack = setupStruct->hMenuStack;
    GuiExecuteOnGuiThread(QtPlugin::Setup);
}

extern "C" __declspec(dllexport) bool plugstop()
{
    GuiExecuteOnGuiThread(QtPlugin::Stop);
    return true;
}
