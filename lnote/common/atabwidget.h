#ifndef ATABWIDGET_H
#define ATABWIDGET_H

#include <QTabWidget>

class QMenu;
class QAction;

class ATabWidget : public QTabWidget
{
	Q_OBJECT
public:
    explicit ATabWidget(QWidget *parent = 0);
    ~ATabWidget();

protected:
    void mousePressEvent(QMouseEvent * event);

private:
    QMenu *rightMouseMenu;
    QAction *closeAction;
    int currentTabIndex = -1;

private slots:
    void slotRightClicked(QAction *action);

signals:
    void signalRightClicked(int index, QString cmd);
};

#endif
