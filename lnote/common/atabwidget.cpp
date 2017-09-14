#include "atabwidget.h"
#include <QMouseEvent>
#include <QTabBar>
#include <QMenu>
#include <QAction>
#include <QDebug>

ATabWidget::ATabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    rightMouseMenu = new QMenu(this);
    closeAction = new QAction(tr("关闭当前文件"));
    closeAction->setObjectName("close");
    rightMouseMenu->addAction(closeAction);
    connect(rightMouseMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotRightClicked(QAction*)));
}

ATabWidget::~ATabWidget()
{
}

void ATabWidget::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::RightButton)
    {
        //获取index，如果获取不到则返回值为-1
        currentTabIndex = this->tabBar()->tabAt(event->pos());
        if(currentTabIndex >= 0)
        {
            rightMouseMenu->exec(QCursor::pos());
        }
    }
    QTabWidget::mousePressEvent(event);
}


void ATabWidget::slotRightClicked(QAction *action)
{
    if(action->text() == "关闭当前文件")
    {
        //qDebug() << "Current index:" << currentTabIndex;
        emit signalRightClicked(currentTabIndex, action->objectName());
    }
}


