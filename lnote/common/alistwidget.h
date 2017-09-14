#ifndef ALISTWIDGET_H
#define ALISTWIDGET_H

#include <QWidget>

class QPushButton;
class QStackWidget;
class QVBoxLayout;

class AListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AListWidget(QWidget *parent = 0);
    ~AListWidget();

public:
    void initLeftListWidget();
    void initRightWidget();

private:
    QPushButton *listButton[6];
    QWidget *leftWidget;
    QStackWidget *rightWidget;
    QVBoxLayout *leftListLayout;

private slots:
    void slotListButtonClicked();
};

#endif
