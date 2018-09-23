#ifndef PLUGINTABWIDGET_H
#define PLUGINTABWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QTabWidget>
#include <QEvent>
#include <QMessageBox>

#include "PluginBookmarkWindow.h"
#include "PluginLabelWindow.h"
#include "PluginCommentWindow.h"
#include "pluginmain.h"

class PluginTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PluginTabWidget(QWidget* parent = 0);

protected:
    void closeEvent(QCloseEvent* event);
    bool eventFilter(QObject* object, QEvent* event);

private slots:
    void on_currentChanged(int index);

private:
    PluginBookmarkWindow* mPluginBookmarkWindow;
    PluginLabelWindow* mPluginLabelWindow;
    PluginCommentWindow* mPluginCommentWindow;
    QTabWidget* tabWidget;
};

#endif // PLUGINTABWIDGET_H
