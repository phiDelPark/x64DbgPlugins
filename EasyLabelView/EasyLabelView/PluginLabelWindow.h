#ifndef PLUGINLABELWINDOW_H
#define PLUGINLABELWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include "common.h"

namespace Ui
{
class PluginLabelWindow;
}

class PluginLabelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PluginLabelWindow(QWidget* parent = nullptr);
    ~PluginLabelWindow();
    void updateLabels();

private slots:
    void tableItemDbClicked(int row, int column);
    void showContextMenu(const QPoint& pos);

private:
    Ui::PluginLabelWindow* ui;
};

#endif // PLUGINLABELWINDOW_H
