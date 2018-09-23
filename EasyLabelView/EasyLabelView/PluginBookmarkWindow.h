#ifndef PLUGINBOOKMARKWINDOW_H
#define PLUGINBOOKMARKWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "common.h"

namespace Ui
{
class PluginBookmarkWindow;
}

class PluginBookmarkWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PluginBookmarkWindow(QWidget* parent = nullptr);
    ~PluginBookmarkWindow();
    void updateBookmarks();

private slots:
    void tableItemDbClicked(int row, int column);
    void showContextMenu(const QPoint& pos);

private:
    Ui::PluginBookmarkWindow* ui;
};

#endif // PLUGINBOOKMARKWINDOW_H
