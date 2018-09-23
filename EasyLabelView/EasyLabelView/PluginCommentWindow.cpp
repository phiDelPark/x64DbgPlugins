#include "PluginCommentWindow.h"
#include "ui_PluginCommentWindow.h"

#include "pluginmain.h"

PluginCommentWindow::PluginCommentWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::PluginCommentWindow)
{
    ui->setupUi(this);

    QTableWidget* tw = ui->tableWidget;

    tw->setFont(parent->font());
    tw->setColumnWidth(0, 115);
    tw->setColumnWidth(1, 185);
    //tw->horizontalHeader()->setStyleSheet("QHeaderView::section { padding: 5px; border: 0px solid gray; border-right-width: 1px; border-bottom-width: 1px; }");

    connect(tw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableItemDbClicked(int,int)));
    tw->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tw, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    //qobject_cast<QTabWidget*>(parentWidget)->setCurrentIndex(0);
}

PluginCommentWindow::~PluginCommentWindow()
{
    delete ui;
}

void PluginCommentWindow::updateComments()
{    
    QTableWidget* tw = ui->tableWidget;

    tw->setFocus();

    LISTINFO       sLISTINFO;
    GETCOMMENTLIST  fGETCOMMENTLIST;

#ifdef COMPILE_X86
    HMODULE hMod = GetModuleHandle(L"x32dbg.dll");
    fGETCOMMENTLIST = (GETCOMMENTLIST)GetProcAddress(hMod, "?GetList@Comment@Script@@YA_NPAUListInfo@@@Z");
#else
    HMODULE hMod = GetModuleHandle(L"x64dbg.dll");
    fGETCOMMENTLIST = (GETCOMMENTLIST)GetProcAddress(hMod, "?GetList@Comment@Script@@YA_NPEAUListInfo@@@Z");
#endif

    memset(&sLISTINFO, 0, sizeof(LISTINFO));

    fGETCOMMENTLIST(&sLISTINFO);

    tw->setRowCount(0);

    if(sLISTINFO.count > 0)
    {
        COMMENTSINFO* list = reinterpret_cast<COMMENTSINFO*>(sLISTINFO.data);

        int row;
        char sAddr[32];

        duint addr = 0;
        BPXTYPE bpxType;
        bool isBookmark;
        char disassembly[GUI_MAX_DISASSEMBLY_SIZE] = "";
        char comment[MAX_COMMENT_SIZE] = "";

        for( int i=0; i<sLISTINFO.count; ++i )
        {
            addr = list[i].addr + DbgFunctions()->ModBaseFromName(list[i].mod);

            bpxType = DbgGetBpxTypeAt(addr);
            isBookmark = DbgGetBookmarkAt(addr);
            GuiGetDisassembly(addr, disassembly);
            DbgGetCommentAt(addr, comment);

            //_plugin_logprintf( "--- %p x\r\n", DbgFunctions()->ModBaseFromName(list[i].mod));
            //_plugin_logprintf( "--- %p - %s - %s x\r\n", addr, disassembly, comment);

            sprintf_s(sAddr, "%p", addr);
            row = tw->rowCount();

            tw->insertRow ( row );
            tw->setItem   ( row, 0, new QTableWidgetItem(sAddr));
            tw->setItem   ( row, 1, new QTableWidgetItem(disassembly));
            tw->setItem   ( row, 2, new QTableWidgetItem(comment));

            QFont font = tw->item(row, 1)->font();
            font.setUnderline(isBookmark);
            tw->item(row, 0)->setFont(font);
            tw->item(row, 0)->setTextColor(
                bpxType == 0
                        ? tw->item(row, 1)->textColor()
                        : COLOR_RED
            );
        }
    }
}

void PluginCommentWindow::tableItemDbClicked(int row, int)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, 0);
    GuiDisasmAt(item->text().toULongLong(0, 16), 0);
}

void PluginCommentWindow::showContextMenu(const QPoint& pos)
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

        QAction* deleteItem = new QAction("Delete comment", this);
        QAction* bookmarkItem = new QAction("Set bookmark", this);
        QAction* unbookmarkItem = new QAction("Remove bookmark", this);
        QAction* breakpointItem = new QAction("Set breakpoint", this);
        QAction* unbreakpointItem = new QAction("Remove breakpoint", this);
        QAction* breakpointAllItem = new QAction("Set breakpoint on all", this);
        QAction* unbreakpointAllItem = new QAction("Remove breakpoint on all", this);

        myMenu.addAction(deleteItem);
        myMenu.addSeparator();
        myMenu.addAction(bpxType == 0 ? breakpointItem : unbreakpointItem);
        myMenu.addAction(breakpointAllItem);
        myMenu.addAction(unbreakpointAllItem);
        myMenu.addSeparator();
        myMenu.addAction(isBookmark ? unbookmarkItem : bookmarkItem);

        QAction* selectedItem = myMenu.exec(tw->mapToGlobal(pos));

        if(selectedItem == deleteItem)
        {
            if(DbgSetCommentAt(addr, ""))
                tw->removeRow(row);
        }
        else if((selectedItem == bookmarkItem) || (selectedItem == unbookmarkItem))
        {
            if(DbgSetBookmarkAt(addr, !isBookmark))
            {
                QFont font = tw->item(row, 1)->font();
                font.setUnderline(!isBookmark);
                tw->item(row, 0)->setFont(font);
            }
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
