#ifndef _PLUGINMAIN_H
#define _PLUGINMAIN_H

#include <windows.h>
#include "pluginsdk\_plugins.h"
#include "QtPlugin.h"
#include <QString>

#define MAX_MODULE_SIZE 256
#ifdef COMPILE_X86
    #define MAX_INFO_TEXT_SIZE 256
#else
    #define MAX_INFO_TEXT_SIZE 264
#endif
#define COLOR_RED 16711680
#define COLOR_BLUE 255

struct LISTINFO
{
    int count; //Number of element in the list.
    size_t size; //Size of list in bytes (used for type checking).
    void* data; //Pointer to the list contents. Must be deleted by the caller using BridgeFree (or List::Free).
};

struct BOOKMARKSINFO
{
    char mod[MAX_MODULE_SIZE];
    uint addr;
#ifndef COMPILE_X86
    char text[8];
#endif
    bool manual;
};

struct LABELSINFO
{
    char mod[MAX_MODULE_SIZE];
    uint addr;
    char text[MAX_INFO_TEXT_SIZE];
    bool manual;
};

struct COMMENTSINFO
{
    char mod[MAX_MODULE_SIZE];
    uint addr;
    char text[MAX_INFO_TEXT_SIZE];
    bool manual;
};

typedef bool (*GETBOOKMARKLLIST)(LISTINFO*);
typedef bool (*GETLABELLIST)(LISTINFO*);
typedef bool (*GETCOMMENTLIST)(LISTINFO*);

bool cmdSetBPX(duint addr, bool set);

namespace Plugin
{
extern int handle;
extern HWND hwndDlg;
extern int hMenu;
extern int hMenuDisasm;
extern int hMenuDump;
extern int hMenuStack;
} //Plugin

#endif //_PLUGINMAIN_H
