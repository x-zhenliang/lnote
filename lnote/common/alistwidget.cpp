#include "alistwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

AListWidget::AListWidget(QWidget *parent)
    :QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    initRightWidget();
    mainLayout->addWidget(leftWidget);
    mainLayout->addStretch();
    mainLayout->setMargin(0);
    setLayout(mainLayout);
}

AListWidget::~AListWidget()
{
}

void AListWidget::initLeftListWidget()
{

}

void AListWidget::initRightWidget()
{
    leftWidget = new QWidget();
    QVBoxLayout *leftVLayout = new QVBoxLayout();
    leftWidget->setLayout(leftVLayout);
    leftWidget->setObjectName("leftWidget");
    leftVLayout->setMargin(0);
    leftVLayout->setSpacing(0);

    listButton[0] = new QPushButton(tr("界面一"));
    listButton[1] = new QPushButton(tr("界面二"));
    listButton[2] = new QPushButton(tr("界面三"));
    listButton[3] = new QPushButton(tr("界面四"));
    listButton[4] = new QPushButton(tr("界面五"));
    listButton[5] = new QPushButton(tr("界面六"));
    for(int i=0; i<6;i++)
    {
       listButton[i]->setFixedSize(100, 50);
       leftVLayout->addWidget(listButton[i]);
    }
    leftVLayout->addStretch();
    QList<QPushButton *> leftButtonList = leftWidget->findChildren<QPushButton *>();
    foreach (QPushButton * button, leftButtonList)
    {
        connect(button, SIGNAL(clicked()), this, SLOT(slotListButtonClicked()));
    }
}

void AListWidget::slotListButtonClicked()
{
    QPushButton *button = (QPushButton *)sender();
    QString name = button->text();
    QList<QPushButton *> btns = leftWidget->findChildren<QPushButton *>();
    foreach (QPushButton * b, btns)
    {
        b->setChecked(false);
    }
    button->setChecked(true);

    qDebug() << "点击的按键为：" << name;
}

