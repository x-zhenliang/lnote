#include "basesetup.h"
#include <QFile>
#include <QApplication>
#include <QColor>
#include <QPalette>
#include <QDebug>

void BaseSetup::setAppStyle(const QString &qssFile)
{
    QFile file(qssFile);
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        file.close();
    }
}




