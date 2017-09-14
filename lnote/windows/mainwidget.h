#ifndef MAINWIDGET_H
#define MAINWIDGET_H

/*
 * Copyright (c) 2003, www.nfschina.com
 * All rights reserved.
 *
 * FileName  :mainwidget.h
 * DocumentID:所属功能块
 * Summary   :实现的主要功能
 * Author    :
 * Date      :
 */

#include <QWidget>
#include <QMap>
class QLabel;
class QPushButton;
class QToolButton;
class QHBoxLayout;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QAction;
class QTabWidget;
class QsciScintilla;
class AsciScintilla;
class ATabWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    //这里只防止构造函数、析构函数
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

public:
    //主布局
    QVBoxLayout *mainVLayout;
    //标题栏
    QLabel *logoLabel;
    QLabel *titleLabel;
    QPushButton *minButton;
    QPushButton *maxButton;
    QPushButton *exitButton;
    QHBoxLayout *topHLayout;
    //菜单栏
    QHBoxLayout *menuHLayout;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *searchMenu;
    QMenu *viewMenu;
    QMenu *inputMenu;
    QMenu *textmodeMenu;
    QMenu *setupMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *historyFileAciton[5];
    QAction *undoAciton;
    QAction *redoAciton;
    QAction *cutAciton;
    QAction *copyAciton;
    QAction *pasteAciton;
    QAction *deleteAciton;
    QAction *searchinCurFileAction;
    QAction *searchinAllFileAction;
    QAction *fullScreenAction;
    QAction *lineNumberAction;
    QAction *inputCHNAction;
    QAction *fontAction;


    //工具栏
    QHBoxLayout *toolBarHLayout;
    QToolButton *userTButton;
    QToolButton *dataTButton;
    QToolButton *setupTButton;

    //主窗体
    QVBoxLayout *centralVBLayout;
    ATabWidget *mainTabWidget;

    //状态栏，位于整个窗体的底部位置

    //窗体移动
    QPoint pressPoint;
    bool isMove;
    bool isMax;
    QRect currentLocation;
    QMap<int, QString> allFile;
    QFont totalFont;



public:
    //设置主界面的布局样式
    void setTopLay();
    void setToolBoxLay();
    void setMenuBarLay();
    void setCentralLay();
    void setBottomLay();


protected:
    //成员变量
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    //成员变量
    //当前文件 文件名-路径
    //QMap<QString, QString> currentFile;
    //后续增加到配置文件中进行读写
    QList<QString> historyFileName;

private:
    //成员函数
    void newFile();
    void openFile();
    void saveFile();
    bool saveFilePath(QString fileName);
    void saveAsFile();
    void closeFile();
    void closeFileByIndex(int index);
    AsciScintilla * createFileTabWidget(QString fileType);

public slots:
    //槽函数

private slots:
    //私有槽函数
    void slotMaxWidget();
    void slotTrigerMenu(QAction *action);
    void slotDocumentModified();
    void slotTabRightClicked(int index, QString cmd);

signals:
    //信号
};
#endif // MAINWIDGET_H


