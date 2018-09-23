#include "PluginTabWidget.h"


PluginTabWidget::PluginTabWidget(QWidget* parent) : QWidget(parent)
{
/*
    QVBoxLayout* layout = parent->findChild<QVBoxLayout *>("mBotLeftFrameLayout");
    QList<QWidget *> widgets = layout->findChildren<QWidget *>();
    foreach (QWidget* b, widgets) {
        //std::cout << b->objectName().toStdString() << std::endl;
        _plugin_logprintf( "--- %s - %d x %d\r\n", b->objectName().toStdString(), b->pos().x(), b->pos().y());
        //QMessageBox::about(this, b->objectName(),b->objectName());
    }
*/

    QVBoxLayout* layout = parent->findChild<QVBoxLayout *>("mBotRightFrameLayout");
    QFrame* frame = parent->findChild<QFrame *>("mBotRightFrame");
    QWidget* stack = frame->childAt(0, 0);

    tabWidget = new QTabWidget;

    mPluginBookmarkWindow = new PluginBookmarkWindow(tabWidget);
    mPluginBookmarkWindow->setAutoFillBackground(true);

    mPluginLabelWindow = new PluginLabelWindow(tabWidget);
    mPluginLabelWindow->setAutoFillBackground(true);

    mPluginCommentWindow = new PluginCommentWindow(tabWidget);
    mPluginCommentWindow->setAutoFillBackground(true);

    tabWidget->addTab(stack, tr("Stack"));
    tabWidget->setTabIcon(0, QIcon(":/icons/images/icon.png"));
    tabWidget->addTab(mPluginBookmarkWindow, tr("Bookmarks"));
    tabWidget->setTabIcon(1, mPluginBookmarkWindow->windowIcon());
    tabWidget->addTab(mPluginLabelWindow, tr("Labels"));
    tabWidget->setTabIcon(2, mPluginLabelWindow->windowIcon());
    tabWidget->addTab(mPluginCommentWindow, tr("Comments"));
    tabWidget->setTabIcon(3, mPluginCommentWindow->windowIcon());

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));

    layout->addWidget(tabWidget);
    layout->setMargin(0);

    QWidget* mTopHSplitter = parent->findChild<QWidget *>("mTopHSplitter");
    QWidget* mTopLeftVSplitter = mTopHSplitter->findChild<QWidget *>("mTopLeftVSplitter");
    QWidget* Disassembly = mTopLeftVSplitter->childAt(187, 0);

    Disassembly->installEventFilter(this);
}

void PluginTabWidget::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);

    mPluginBookmarkWindow->close();
    mPluginLabelWindow->close();
    mPluginCommentWindow->close();
}

bool PluginTabWidget::eventFilter(QObject*, QEvent* event)
{
    if (event->type() == QEvent::FocusIn)
    {
        //qobject_cast<QTabWidget*>(parentWidget)->setCurrentIndex(0);
        if(tabWidget->currentIndex() != 0)
            tabWidget->setCurrentIndex(0);
    }
    return false;
}

void PluginTabWidget::on_currentChanged(int index)
{
    switch (index)
    {
    case 0:
      break;
    case 1:
        mPluginBookmarkWindow->updateBookmarks();
      break;
    case 2:
        mPluginLabelWindow->updateLabels();
      break;
    case 3:
        mPluginCommentWindow->updateComments();
      break;
    }
}
