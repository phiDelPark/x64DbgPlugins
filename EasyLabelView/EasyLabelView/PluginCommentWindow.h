#ifndef PLUGINCOMMENTWINDOW_H
#define PLUGINCOMMENTWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "common.h"

namespace Ui
{
class PluginCommentWindow;
}

class PluginCommentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PluginCommentWindow(QWidget* parent = nullptr);
    ~PluginCommentWindow();
    void updateComments();

private slots:
    void tableItemDbClicked(int row, int column);
    void showContextMenu(const QPoint& pos);

private:
    Ui::PluginCommentWindow* ui;
};

#endif // PLUGINCOMMENTWINDOW_H
