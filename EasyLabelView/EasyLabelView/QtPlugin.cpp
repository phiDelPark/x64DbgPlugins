#include "QtPlugin.h"

//#include "pluginmain.h"
#include "PluginTabWidget.h"

static PluginTabWidget* pluginTabWidget;
static HANDLE hSetupEvent;
static HANDLE hStopEvent;

static QWidget* getParent()
{
    return QWidget::find((WId)Plugin::hwndDlg);
}

void QtPlugin::Init()
{
    hSetupEvent = CreateEventW(nullptr, true, false, nullptr);
    hStopEvent = CreateEventW(nullptr, true, false, nullptr);
}

void QtPlugin::Setup()
{
    QWidget* parent = getParent();
    pluginTabWidget = new PluginTabWidget(parent);
    SetEvent(hSetupEvent);
}

void QtPlugin::WaitForSetup()
{
    WaitForSingleObject(hSetupEvent, INFINITE);
}

void QtPlugin::Stop()
{
    delete pluginTabWidget;
    SetEvent(hStopEvent);
}

void QtPlugin::WaitForStop()
{
    WaitForSingleObject(hStopEvent, INFINITE);
}

void QtPlugin::ShowTab()
{
}
