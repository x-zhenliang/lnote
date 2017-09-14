#ifndef BASESETUP_H
#define BASESETUP_H

#include <QObject>


class BaseSetup : public QObject
{
public:
    static void setAppStyle(const QString &qssFile);

};
#endif
