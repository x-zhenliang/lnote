#include "windows/mainwidget.h"
#include "common/basesetup.h"
#include "common/alistwidget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget mainWidget;
    BaseSetup::setAppStyle(":/qss/blue");
    mainWidget.show();

//    AListWidget alist;
//    BaseSetup::setAppStyle(":/qss/blue");
//    alist.show();
    return a.exec();
}


