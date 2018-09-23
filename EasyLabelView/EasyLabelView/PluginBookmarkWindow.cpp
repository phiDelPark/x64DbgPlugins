#include "PluginBookmarkWindow.h"
#include "ui_PluginBookmarkWindow.h"

#include "pluginmain.h"

PluginBookmarkWindow::PluginBookmarkWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::PluginBookmarkWindow)
{
    ui->setupUi(this);

    QTableWidget* tw = ui->tableWidget;

    tw->setFont(parent->font());
    tw->setColumnWidth(0, 115);
    tw->setColumnWidth(1, 185);

    connect(tw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableItemDbClicked(int,int)));
    tw->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tw, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

PluginBookmarkWindow::~PluginBookmarkWindow()
{
    delete ui;
}

void PluginBookmarkWindow::updateBookmarks()
{    
    QTableWidget* tw = ui->tableWidget;

    tw->setFocus();

    LISTINFO       sLISTINFO;
    GETBOOKMARKLLIST  fGETBOOKMARKLLIST;

#ifdef COMPILE_X86
    HMODULE hMod = GetModuleHandle(L"x32dbg.dll");
    fGETBOOKMARKLLIST = (GETBOOKMARKLLIST)GetProcAddress(hMod, "?GetList@Bookmark@Script@@YA_NPAUListInfo@@@Z");
#else
    HMODULE hMod = GetModuleHandle(L"x64dbg.dll");
    fGETBOOKMARKLLIST = (GETBOOKMARKLLIST)GetProcAddress(hMod, "?GetList@Bookmark@Script@@YA_NPEAUListInfo@@@Z");
#endif

    memset(&sLISTINFO, 0, sizeof(LISTINFO));

    fGETBOOKMARKLLIST(&sLISTINFO);

/*
    _plugin_logprintf( "--- %d x\r\n", sizeof(BOOKMARKSINFO));
    _plugin_logprintf( "--- %d x\r\n", sLISTINFO.count);
    _plugin_logprintf( "--- %d x\r\n", sLISTINFO.size);
*/

    tw->setRowCount(0);

    if(sLISTINFO.count > 0)
    {
        BOOKMARKSINFO* list = reinterpret_cast<BOOKMARKSINFO*>(sLISTINFO.data);

        int row;
        char sAddr[20];

        duint addr = 0;
        BPXTYPE bpxType;
        bool isLabel;
        bool isComment;
        char disassembly[GUI_MAX_DISASSEMBLY_SIZE] = "";
        char label[MAX_LABEL_SIZE] = "";
        char comment[MAX_COMMENT_SIZE] = "";

        for( int i=0; i<sLISTINFO.count; ++i )
        {
            addr = list[i].addr + DbgFunctions()->ModBaseFromName(list[i].mod);

            sprintf_s(sAddr, "%p", addr);
            row = tw->rowCount();

            bpxType = DbgGetBpxTypeAt(addr);
            GuiGetDisassembly(addr, disassembly);

            strcpy(label, "");
            strcpy(comment, "");

            isLabel = DbgGetLabelAt(addr, SEG_DEFAULT, label);
            isComment = DbgGetCommentAt(addr, comment);

            tw->insertRow ( row );
            tw->setItem   ( row, 0, new QTableWidgetItem(sAddr));
            tw->setItem   ( row, 1, new QTableWidgetItem(disassembly));
            tw->setItem   ( row, 2, new QTableWidgetItem(isComment ? comment : label));

            tw->item(row, 0)->setTextColor(bpxType == 0 ? tw->item(row, 1)->textColor() : COLOR_RED);

            QFont font = tw->item(row, 1)->font();
            font.setUnderline(isLabel);
            tw->item(row, 2)->setFont(font);
        }
    }
}

void PluginBookmarkWindow::tableItemDbClicked(int row, int)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item = ui->tableWidget->item(row, 0);
    GuiDisasmAt(item->text().toULongLong(0, 16), 0);
}

void PluginBookmarkWindow::showContextMenu(const QPoint& pos)
{
    int row = ui->tableWidget->currentRow();

    if(row > -1)
    {
        QMenu myMenu;
        QTableWidget* tw = ui->tableWidget;
        duint addr = tw->item(row, 0)->text().toULongLong(0, 16);

        BPXTYPE bpxType;
        bool isBookmark;
        bpxType = DbgGetBpxTypeAt(addr);
        isBookmark = DbgGetBookmarkAt(addr);

        QAction* deleteItem = new QAction("Remove bookmark", this);
        QAction* breakpointItem = new QAction("Set breakpoint", this);
        QAction* unbreakpointItem = new QAction("Remove breakpoint", this);
        QAction* breakpointAllItem = new QAction("Set breakpoint on all", this);
        QAction* unbreakpointAllItem = new QAction("Remove breakpoint on all", this);

        myMenu.addAction(deleteItem);
        myMenu.addSeparator();
        myMenu.addAction(bpxType == 0 ? breakpointItem : unbreakpointItem);
        myMenu.addAction(breakpointAllItem);
        myMenu.addAction(unbreakpointAllItem);

        QAction* selectedItem = myMenu.exec(tw->mapToGlobal(pos));

        if(selectedItem == deleteItem)
        {
            if(DbgSetBookmarkAt(addr, false))
                ui->tableWidget->removeRow(row);
        }
        else if((selectedItem == breakpointItem) || (selectedItem == unbreakpointItem))
        {
            if(cmdSetBPX(addr, bpxType == 0))
                tw->item(row, 0)->setTextColor(
                    DbgGetBpxTypeAt(addr) == 0
                            ? tw->item(row, 1)->textColor()
                            : COLOR_RED
                );
        }
        else if((selectedItem == breakpointAllItem) || (selectedItem == unbreakpointAllItem))
        {
            for( int i=0; i<tw->rowCount(); ++i )
            {
                addr = addr = tw->item(i, 0)->text().toULongLong(0, 16);

                if(cmdSetBPX(addr, selectedItem == breakpointAllItem))
                    tw->item(i, 0)->setTextColor(
                        DbgGetBpxTypeAt(addr) == 0
                                ? tw->item(i, 1)->textColor()
                                : COLOR_RED
                    );
            }
        }
    }
}
