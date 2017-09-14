#include "mainwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTabWidget>
#include <QDebug>
#include <QTextEdit>
#include <QTextCodec>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QFontDatabase>
#include "common/atabwidget.h"
#include "qsciscintilla.h"
#include "qscidocument.h"
#include "qscilexercpp.h"
#include "asciscintilla.h"
#include "common/basedefine.h"
#include "inputchinese/inputchinese.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),
      isMax(false)
{

    resize(800, 600);
    setWindowIcon(QIcon(":/title/logo"));
    setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    //setup font

    mainVLayout = new QVBoxLayout();
    setTopLay();
    setMenuBarLay();
    setToolBoxLay();
    setCentralLay();
    mainVLayout->setSpacing(0);
    mainVLayout->addLayout(topHLayout);
    mainVLayout->addLayout(menuHLayout);
    mainVLayout->addLayout(centralVBLayout);
    //mainVLayout->addLayout(toolBarHLayout);
    //mainVLayout->addStretch();
    mainVLayout->setContentsMargins(0 ,0 ,0 ,1);
    mainVLayout->setMargin(0);
    setLayout(mainVLayout);
    currentLocation = this->geometry();
}

MainWidget::~MainWidget()
{

}

void MainWidget::setTopLay()
{
    minButton = new QPushButton();
    maxButton = new QPushButton();
    exitButton = new QPushButton();
    topHLayout = new QHBoxLayout();

    titleLabel = new QLabel();
    logoLabel = new QLabel();
    titleLabel->setObjectName("titleLabel");
    titleLabel->setText(tr("LNote"));
    logoLabel->setPixmap(QPixmap(":/title/logo"));
    logoLabel->setFixedSize(20, 12);
    minButton->setObjectName("minButton");
    maxButton->setObjectName("maxButton");
    exitButton->setObjectName("exitButton");

    QHBoxLayout *titleHBLayout = new QHBoxLayout();
    QHBoxLayout *buttonHBLayout = new QHBoxLayout();
    titleHBLayout->addWidget(logoLabel);
    titleHBLayout->addSpacing(5);
    titleHBLayout->addWidget(titleLabel);
    titleHBLayout->setContentsMargins(5, 5, 0 ,0);

    buttonHBLayout->addWidget(minButton);
    buttonHBLayout->addWidget(maxButton);
    buttonHBLayout->addWidget(exitButton);

    topHLayout->addLayout(titleHBLayout);
    topHLayout->addStretch();
    topHLayout->addLayout(buttonHBLayout);
    topHLayout->setContentsMargins(0,0,0,0);
    connect(minButton,SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(maxButton,SIGNAL(clicked()), this, SLOT(slotMaxWidget()));
    connect(exitButton,SIGNAL(clicked()), this, SLOT(close()));
}

void MainWidget::setToolBoxLay()
{
    userTButton = new QToolButton();
    dataTButton = new QToolButton();
    setupTButton = new QToolButton();
    userTButton->setObjectName("userTButton");
    dataTButton->setObjectName("dataTButton");
    setupTButton->setObjectName("setupTButton");
    userTButton->setIcon(QPixmap(":/title/user"));
    dataTButton->setIcon(QPixmap(":/title/computer"));
    setupTButton->setIcon(QPixmap(":/title/setup"));
    userTButton->setIconSize(QSize(44, 44));
    dataTButton->setIconSize(QSize(44, 44));
    setupTButton->setIconSize(QSize(44, 44));
    userTButton->setText(tr("用户"));
    dataTButton->setText(tr("数据"));
    setupTButton->setText(tr("设置"));
    userTButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    dataTButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setupTButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    toolBarHLayout = new QHBoxLayout();
    toolBarHLayout->addWidget(userTButton);
    toolBarHLayout->addWidget(dataTButton);
    toolBarHLayout->addWidget(setupTButton);
    toolBarHLayout->addStretch();
}

void MainWidget::setMenuBarLay()
{
    menuHLayout = new QHBoxLayout();
    menuBar = new QMenuBar();
    fileMenu = new QMenu(tr("文件"));    
    editMenu = new QMenu(tr("编辑"));
    searchMenu = new QMenu(tr("搜索"));
    viewMenu  = new QMenu(tr("视图"));
    textmodeMenu = new QMenu(tr("文本模式"));
    setupMenu = new QMenu(tr("设置"));

    newAction = new QAction(tr("新建"));
    openAction = new QAction(tr("打开"));
    saveAction = new QAction(tr("保存"));
    saveAsAction = new QAction(tr("另存为"));
    closeAction = new QAction(tr("关闭"));
    undoAciton = new QAction(tr("撤销"));
    redoAciton = new QAction(tr("恢复"));
    cutAciton  = new QAction(tr("剪切"));
    copyAciton = new QAction(tr("复制"));
    pasteAciton = new QAction(tr("粘贴"));
    deleteAciton = new QAction(tr("删除"));
    searchinCurFileAction = new QAction(tr("在当前文件搜索"));
    searchinAllFileAction = new QAction(tr("在所有打开文件中搜索"));
    fullScreenAction = new QAction(tr("切换到全屏模式"));
    lineNumberAction = new QAction(tr("显示行号"));    
    inputMenu = new QMenu(tr("中文"));
    inputCHNAction = new QAction(tr("虚拟中文输入法"));
    fontAction = new QAction(tr("字体"));

    QStringList textMode;
    textMode << "Bash" << "Cpp" << "Css" << "Java" << "Javascript" \
             << "Json" << "Html" << "Lua" << "Perl" << "Fortan" \
             << "SQL" << "XML" << "Yaml" << "Makefile" << "Cmake";

    int count = textMode.count();
    QAction *textAcitons[count];
    for(int i = 0; i< count; i++)
    {
        textAcitons[i] = new QAction(textMode.at(i));
        textmodeMenu->addAction(textAcitons[i]);
    }
    inputMenu->addAction(inputCHNAction);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeAction);
    editMenu->addAction(undoAciton);
    editMenu->addAction(redoAciton);
    editMenu->addAction(cutAciton);
    editMenu->addAction(copyAciton);
    editMenu->addAction(pasteAciton);
    editMenu->addAction(deleteAciton);
    searchMenu->addAction(searchinCurFileAction);
    searchMenu->addAction(searchinAllFileAction);
    viewMenu->addAction(fullScreenAction);
    viewMenu->addAction(lineNumberAction);
    setupMenu->addAction(fontAction);
    menuBar->setObjectName("mainMenuBar");
    fileMenu->setObjectName("fileMenu");
    editMenu->setObjectName("editMenu");
    searchMenu->setObjectName("searchMenu");
    viewMenu->setObjectName("viewMenu");
    inputMenu->setObjectName("inputMenu");
    textmodeMenu->setObjectName("textmodeMenu");
    setupMenu->setObjectName("setupMenu");
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(searchMenu);
    menuBar->addMenu(viewMenu);
    menuBar->addMenu(inputMenu);
    menuBar->addMenu(textmodeMenu);
    menuBar->addMenu(setupMenu);
    menuHLayout->addWidget(menuBar);
    menuHLayout->addSpacing(2);
    menuHLayout->setContentsMargins(1, 0, 2, 0);
    connect(menuBar,SIGNAL(triggered(QAction*)),this,SLOT(slotTrigerMenu(QAction*)));
}

void MainWidget::setCentralLay()
{
    centralVBLayout = new QVBoxLayout();
    mainTabWidget = new ATabWidget();
    connect(mainTabWidget, SIGNAL(signalRightClicked(int, QString)), this, SLOT(slotTabRightClicked(int, QString)));
    AsciScintilla *textEdit = new AsciScintilla();
    textEdit->setObjectName("new1");
    textEdit->textEdit->setFont(totalFont);
    mainTabWidget->addTab(textEdit, tr("new1"));
    //mainTabWidget->setTabIcon(0, QIcon(":/title/logo"));
    centralVBLayout->addWidget(mainTabWidget);
    centralVBLayout->setContentsMargins(1, 0, 1, 0);
}

void MainWidget::setBottomLay()
{

}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    pressPoint = event->pos();
    isMove = true;
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && isMove)
    {
        QPoint currentPoint = this->pos();
        currentPoint.setX(currentPoint.x() + event->x() - pressPoint.x());
        currentPoint.setY(currentPoint.y() + event->y() - pressPoint.y());
        this->move(currentPoint);
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *)
{
    isMove = false;
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painterFrame(this);
    painterFrame.setPen(0xC0EBD7);
    painterFrame.drawRect(0, 0, this->width()-1, this->height()-1);
//    QPainter painterBackGround(this);
//    painterBackGround.drawPixmap(rect(),QPixmap(":/title/background"));
//    以下注释部分用于调整窗体中间的颜色，使用画笔画出矩形区域
//    QPainter painterMain(this);
//    QLinearGradient linearMain(rect().topLeft(), rect().bottomLeft());
//    linearMain.setColorAt(0, Qt::white);
//    linearMain.setColorAt(0.5, Qt::white);
//    linearMain.setColorAt(1, Qt::white);
//    painterMain.setPen(Qt::white); //设定边框颜色
//    painterMain.setBrush(linearMain); //Pen设置的是边框的颜色，Brush设置的方框内部的颜色
//    painterMain.drawRect(QRect(0, 109, this->width()-1, this->height()-109-1));
}


void MainWidget::slotMaxWidget()
{
    if(isMax)
    {
        this->setGeometry(currentLocation);
        maxButton->setToolTip(tr("最大化"));
    }
    else
    {
        currentLocation = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        maxButton->setToolTip(tr("还原"));
    }
    isMax = !isMax;
}

void MainWidget::slotTrigerMenu(QAction *action)
{
    //判断方法一
    QString textContent = action->text();
    qDebug() << textContent;
    if(textContent == "新建")
    {
        newFile();
    }
    else if(textContent == "打开")
    {
        openFile();
    }
    else if(textContent == "关闭")
    {
        closeFile();
    }
    else if(textContent == "保存")
    {
        saveFile();
    }
    else if(textContent == "另存为")
    {
        saveAsFile();
    }
}


void MainWidget::slotDocumentModified()
{
}

void MainWidget::newFile()
{
    int begin;
    QVector<int> indexAll;
    int count = mainTabWidget->count();
    for(int i = 0; i < count; i++)
    {
        QString tmpfileName = mainTabWidget->widget(i)->objectName();
        if(tmpfileName.startsWith("new"))
        {
            int index = tmpfileName.remove("new").toInt();
            indexAll.append(index);
        }
    }
    int vectorCount = indexAll.count();
    if(vectorCount > 0)
    {
        int max;
        max = indexAll[0];
        for(int i= 0; i < vectorCount; i++)
        {
            if(indexAll[i] > max)
            {
                max = indexAll[i];
            }
        }
        begin = max + 1;
    }
    else
    {
        begin = 1;
    }

    QString displayName = QString("new%1").arg(begin);
    AsciScintilla *textEdit = new AsciScintilla();
    textEdit->setObjectName(displayName);
    mainTabWidget->addTab(textEdit, displayName);
    mainTabWidget->setCurrentIndex(mainTabWidget->count()-1);

}

void MainWidget::openFile()
{
    bool sameFileFlag = true;
    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;


    QFile file(fileName);
    QFileInfo fileInfo(fileName);
    if (!file.open(QFile::ReadOnly))
    {
       //权限不足的错误
       QMessageBox::warning(this, tr("Application"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
       return;
    }

    //增加打开相同文件的判断方法
    qDebug() << "AllFile" << allFile;
    if(!allFile.isEmpty())
    {
        qDebug() << allFile;
        int index;
        QList<QString> textName = allFile.values();
        foreach(QString filepath, textName)
        {
            if(filepath == fileName )
            {
               index = allFile.key(filepath);
               mainTabWidget->setCurrentIndex(index);
               sameFileFlag = false;
            }
            else
            {
                 sameFileFlag = true;
            }
        }
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(sameFileFlag)
    {

        AsciScintilla *editor;
        QFileInfo tmpFileInfo(fileName);
        QString filename = tmpFileInfo.fileName();
        if(filename.contains("."))
        {
            QStringList nameList = tmpFileInfo.fileName().split(".");
            editor = createFileTabWidget(nameList[1]);
        }
        else
        {
            editor = createFileTabWidget("");
        }
        editor->textEdit->setText(in.readAll());
        editor->setObjectName(fileInfo.fileName());
        mainTabWidget->addTab(editor, fileInfo.fileName());
        mainTabWidget->setCurrentIndex(mainTabWidget->count()-1);
    }
    QApplication::restoreOverrideCursor();
    allFile.insert(mainTabWidget->currentIndex(),fileName);
    //如果文件过大就进行切分，切分之后显示，后续实现一个完整的机制出来
}

AsciScintilla * MainWidget::createFileTabWidget(QString fileType)
{
    qDebug() << fileType;
    AsciScintilla *editor;
    if(fileType.isEmpty())
    {
       editor = new AsciScintilla();
    }
    else if(fileType == "c"
            || fileType == "C"
            || fileType == "cpp"
            || fileType == "Cpp"
            || fileType == "CPP"
            || fileType == "h")
    {
        editor = new AsciScintilla(CPP_TEXT);
    }
    else
    {
        editor = new AsciScintilla();
    }
    return editor;
}
void MainWidget::saveFile()
{   
    QString currentFileName;
    int index = mainTabWidget->currentIndex();
    if(allFile.contains(index))
    {
        currentFileName = allFile.value(index);
    }
    else
    {
         AsciScintilla *tmpEditor = static_cast< AsciScintilla *>(mainTabWidget->widget(index));
         currentFileName = tmpEditor->objectName();
    }

    if(currentFileName.startsWith("new"))
    {
        saveAsFile();
    }
    else
    {
        bool saveFlag = saveFilePath(currentFileName);
        if(!saveFlag)
        {
            //保存失败，请检查是否存在权限问题
        }
    }
    qDebug() << currentFileName;

}

bool MainWidget::saveFilePath(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QMessageBox::warning(this, tr("Application"),
                         tr("Cannot write file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    int index = mainTabWidget->currentIndex();
    AsciScintilla *tmpEditor = static_cast< AsciScintilla *>(mainTabWidget->widget(index));
    out  << tmpEditor->textEdit->text();
    QApplication::restoreOverrideCursor();
    return true;
}

void MainWidget::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
    {
        return;
    }
    qDebug() << fileName;
    QFileInfo fileInfo(fileName);
    int index = mainTabWidget->currentIndex();
    mainTabWidget->setTabText(index, fileInfo.fileName());
    //添加到allFile
    allFile.insert(index, fileName);
    saveFilePath(fileName);
}

void MainWidget::closeFile()
{
    int currentIndex = mainTabWidget->currentIndex();
    closeFileByIndex(currentIndex);
}

void MainWidget::closeFileByIndex(int index)
{
    AsciScintilla *tmpEditor = static_cast< AsciScintilla *>(mainTabWidget->widget(index));
    mainTabWidget->removeTab(index);
    //判断当前页面中是否有内容，如果有提醒用户保存；如果没有直接关闭即可。
    //文档内容发生变动的标志
    //页面窗体不再使用就主动把他销毁，防止占用过多的内存
    delete tmpEditor;
}


void MainWidget::slotTabRightClicked(int index, QString cmd)
{
    qDebug() << "right  Clicked:";
    qDebug() << index;
    qDebug() << cmd;
    if(cmd == "close")
    {
        closeFileByIndex(index);
    }
}

//函数的注释说明
/*
 * Brief ：
 * Param ：
 * Param ：
 * Return：
 */
//#ifndef MMCP_NO_DEBUG
//	qDebug(“debug message”);
//	QMessageBox::warning(0,“warning”,”message”);
//#endif

