#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "inputchinese.h"
#include "qsciscintilla.h"
//#include "ui_frminputnew.h"

InputChinese *InputChinese::_self = 0;
InputChinese::InputChinese(QWidget *parent) :
    QWidget(parent)
{
    //中文输入法的调用方法
    //InputChinese::Instance()->init("center", "black", 14 + 2, 14, 700, 230, 20, 20, 6, 45);
    //初始化输入法的窗体
    this->initFormbyCode();
    this->initProperty();
    this->initForm();
}

InputChinese::~InputChinese()
{

}

void InputChinese::init(QString position, QString style, int btnFontSize, int labFontSize, int frmWidth, int frmHeight, int icoWidth, int icoHeight, int spacing, int topHeight)
{
    this->currentPosition = position;
    this->currentStyle = style;
    this->btnFontSize = btnFontSize;
    this->labFontSize = labFontSize;
    this->frmWidth = frmWidth;
    this->frmHeight = frmHeight;
    this->icoWidth = icoWidth;
    this->icoHeight = icoHeight;
    this->spacing = spacing;
    this->topHeight = topHeight;
    this->changeSize();
    this->changeStyle();
}

void InputChinese::init(QString position)
{
    this->currentPosition = position;
}

void InputChinese::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void InputChinese::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void InputChinese::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void InputChinese::initForm()
{
    this->mousePressed = false;
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    frmWidth = 600;
    frmHeight = 200;
    icoWidth = 20;
    icoHeight = 20;
    spacing = 6;
    topHeight = 40;

    QDesktopWidget desk;
    deskWidth = desk.availableGeometry().width();
    deskHeight = desk.availableGeometry().height();

    QSqlDatabase dbConn;
    dbConn = QSqlDatabase::addDatabase("QSQLITE", "py");
    dbConn.setDatabaseName(qApp->applicationDirPath() + "/db/py.db");
    dbConn.open();

    isFirst = true;
    isPress = false;
    timerPress = new QTimer(this);
    connect(timerPress, SIGNAL(timeout()), this, SLOT(reClicked()));

    currentWidget = 0;
    currentLineEdit = 0;
    currentTextEdit = 0;
    currentPlain = 0;
    currentBrowser = 0;
    currentEditType = "";

    //如果需要更改输入法面板的显示位置,改变currentPosition这个变量即可
    //control--显示在对应输入框的正下方 bottom--填充显示在底部  center--窗体居中显示
    currentPosition = "";

    //如果需要更改输入法面板的样式,改变currentStyle这个变量即可
    //blue--淡蓝色   black--黑色  brown--灰黑色  gray--灰色  silvery--银色
    currentStyle = "";

    upper = false;

    //输入法面板字体大小,如果需要更改面板字体大小,该这里即可
    btnFontSize = 10;
    labFontSize = 10;

    //如果需要更改输入法初始模式,改变currentType这个变量即可
    //english--英文模式  chinese--中文模式  handwrite--手写模式  char--特殊字符模式
    currentType = "english";
    changeType(currentType);

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();

    foreach (QPushButton *b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(btnClicked()));
    }

    //绑定全局改变焦点信号槽
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(focusChanged(QWidget *, QWidget *)));
    //绑定按键事件过滤器
    qApp->installEventFilter(this);
}

void InputChinese::initProperty()
{
    this->btnDot->setProperty("btnOther", true);
    this->btnSpace->setProperty("btnOther", true);
    this->btnEnter->setProperty("btnOther", true);
    this->btnDelete->setProperty("btnOther", true);
    this->btnUpper->setProperty("btnOther", true);

    this->btn0->setProperty("btnNum", true);
    this->btn1->setProperty("btnNum", true);
    this->btn2->setProperty("btnNum", true);
    this->btn3->setProperty("btnNum", true);
    this->btn4->setProperty("btnNum", true);
    this->btn5->setProperty("btnNum", true);
    this->btn6->setProperty("btnNum", true);
    this->btn7->setProperty("btnNum", true);
    this->btn8->setProperty("btnNum", true);
    this->btn9->setProperty("btnNum", true);

    this->btna->setProperty("btnLetter", true);
    this->btnb->setProperty("btnLetter", true);
    this->btnc->setProperty("btnLetter", true);
    this->btnd->setProperty("btnLetter", true);
    this->btne->setProperty("btnLetter", true);
    this->btnf->setProperty("btnLetter", true);
    this->btng->setProperty("btnLetter", true);
    this->btnh->setProperty("btnLetter", true);
    this->btni->setProperty("btnLetter", true);
    this->btnj->setProperty("btnLetter", true);
    this->btnk->setProperty("btnLetter", true);
    this->btnl->setProperty("btnLetter", true);
    this->btnm->setProperty("btnLetter", true);
    this->btnn->setProperty("btnLetter", true);
    this->btno->setProperty("btnLetter", true);
    this->btnp->setProperty("btnLetter", true);
    this->btnq->setProperty("btnLetter", true);
    this->btnr->setProperty("btnLetter", true);
    this->btns->setProperty("btnLetter", true);
    this->btnt->setProperty("btnLetter", true);
    this->btnu->setProperty("btnLetter", true);
    this->btnv->setProperty("btnLetter", true);
    this->btnw->setProperty("btnLetter", true);
    this->btnx->setProperty("btnLetter", true);
    this->btny->setProperty("btnLetter", true);
    this->btnz->setProperty("btnLetter", true);

    labCh.append(this->labCh0);
    labCh.append(this->labCh1);
    labCh.append(this->labCh2);
    labCh.append(this->labCh3);
    labCh.append(this->labCh4);
    labCh.append(this->labCh5);
    labCh.append(this->labCh6);
    labCh.append(this->labCh7);
    labCh.append(this->labCh8);
    labCh.append(this->labCh9);

    for (int i = 0; i < 10; i++) {
        labCh.at(i)->installEventFilter(this);
    }
}

void InputChinese::showPanel()
{
    this->setVisible(true);
    int width = this->btna->width();
    width = width > 45 ? width : 45;
    this->btnPre->setFixedWidth(width);
    this->btnNext->setFixedWidth(width);
}

//事件过滤器,用于识别鼠标单击汉字标签处获取对应汉字
bool InputChinese::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            if (obj == this->labCh0) {
                setChinese(0);
            } else if (obj == this->labCh1) {
                setChinese(1);
            } else if (obj == this->labCh2) {
                setChinese(2);
            } else if (obj == this->labCh3) {
                setChinese(3);
            } else if (obj == this->labCh4) {
                setChinese(4);
            } else if (obj == this->labCh5) {
                setChinese(5);
            } else if (obj == this->labCh6) {
                setChinese(6);
            } else if (obj == this->labCh7) {
                setChinese(7);
            } else if (obj == this->labCh8) {
                setChinese(8);
            } else if (obj == this->labCh9) {
                setChinese(9);
            } else if (currentEditType != "" && obj != this->btnClose) {
                QString objName = obj->objectName();

                if (obj->parent() != 0x0 && !obj->property("noinput").toBool() && objName != "frmMainWindow"
                        && objName != "frmInputWindow" && objName != "qt_edit_menu" && objName != "labPY") {
                    if (obj->inherits("QGroupBox") || obj->inherits("QFrame") || obj->inherits("QMenu")) {
                        this->hide();
                    } else {
                        showPanel();
                    }
                }
            }

            btnPress = (QPushButton *)obj;

            if (checkPress()) {
                isPress = true;
                timerPress->start(500);
            }

            return false;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        btnPress = (QPushButton *)obj;

        if (checkPress()) {
            isPress = false;
            timerPress->stop();
        }

        return false;
    } else if (event->type() == QEvent::KeyPress) {
        //如果输入法窗体不可见,则不需要处理
        if (!isVisible()) {
            return QWidget::eventFilter(obj, event);
        }
        this->labPY->setText("test");
        QString labText = this->labPY->text();
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        //Shift切换输入法模式,esc键关闭输入法面板,空格取第一个汉字,退格键删除
        //中文模式下回车键取拼音,中文模式下当没有拼音时可以输入空格
        if (keyEvent->key() == Qt::Key_Space) {
            if (labText != "") {
                setChinese(0);
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            if (labText != "") {
                insertValue(labText);
                this->labPY->setText("");
                selectChinese();
            } else {
                this->setVisible(false);
            }

            return true;
        } else if (keyEvent->key() == Qt::Key_Shift) {
            this->btnType->click();
            return true;
        } else if (keyEvent->key() == Qt::Key_Escape) {
            this->btnClose->click();
            return true;
        } else if (keyEvent->key() == Qt::Key_Backspace) {
            this->btnDelete->click();
            return true;
        } else if (keyEvent->text() == "+" || keyEvent->text() == "=") {
            if (labText != "") {
                this->btnNext->click();
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->text() == "-" || keyEvent->text() == "_") {
            if (labText != "") {
                this->btnPre->click();
                return true;
            } else {
                return false;
            }
        } else if (keyEvent->key() == Qt::Key_CapsLock) {
            this->btnUpper->click();
            return true;
        } else {
            if (currentEditType == "QWidget") {
                return false;
            }

            QString key;

            if (currentType == "chinese") {
                key = keyEvent->text();
            } else if (currentType == "english") {
                if (upper) {
                    key = keyEvent->text().toUpper();
                } else {
                    key = keyEvent->text().toLower();
                }
            } else if (currentType == "handwrite") {
            }

            if (!key.isEmpty()) {
                QList<QPushButton *> btn = this->findChildren<QPushButton *>();

                foreach (QPushButton *b, btn) {
                    QString text = b->text();

                    if (!text.isEmpty() && text == key) {
                        b->click();
                        return true;
                    }
                }
            }
        }

        return false;
    }

    return QWidget::eventFilter(obj, event);
}

bool InputChinese::checkPress()
{
    //只有属于输入法键盘的合法按钮才继续处理
    bool num_ok = btnPress->property("btnNum").toBool();
    bool other_ok = btnPress->property("btnOther").toBool();
    bool letter_ok = btnPress->property("btnLetter").toBool();

    if (num_ok || other_ok || letter_ok) {
        return true;
    }

    return false;
}

void InputChinese::reClicked()
{
    if (isPress) {
        timerPress->setInterval(30);
        btnPress->click();
    }
}

void InputChinese::focusChanged(QWidget *oldWidget, QWidget *nowWidget)
{
    qDebug() << "oldWidget:" << oldWidget << " nowWidget:" << nowWidget;
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget)) {
        //在Qt5和linux系统中(嵌入式linux除外),当输入法面板关闭时,焦点会变成无,然后焦点会再次移到焦点控件处
        //这样导致输入法面板的关闭按钮不起作用,关闭后马上有控件获取焦点又显示.
        //为此,增加判断,当焦点是从有对象转为无对象再转为有对象时不要显示.
        //这里又要多一个判断,万一首个窗体的第一个焦点就是落在可输入的对象中,则要过滤掉
#ifndef __arm__
        if (oldWidget == 0x0 && !isFirst) {
            QTimer::singleShot(0, this, SLOT(hide()));
            return;
        }

#endif

        //如果对应属性noinput为真则不显示
        if (nowWidget->property("noinput").toBool()) {
            QTimer::singleShot(0, this, SLOT(hide()));
            return;
        }

        isFirst = false;

        if (nowWidget->inherits("QLineEdit")) {
            currentLineEdit = (QLineEdit *)nowWidget;
            currentEditType = "QLineEdit";
            showPanel();
        } else if (nowWidget->inherits("QTextEdit")) {
            currentTextEdit = (QTextEdit *)nowWidget;
            currentEditType = "QTextEdit";
            showPanel();
        } else if (nowWidget->inherits("QPlainTextEdit")) {
            currentPlain = (QPlainTextEdit *)nowWidget;
            currentEditType = "QPlainTextEdit";
            showPanel();
        } else if (nowWidget->inherits("QTextBrowser")) {
            currentBrowser = (QTextBrowser *)nowWidget;
            currentEditType = "QTextBrowser";
            showPanel();
        } else if (nowWidget->inherits("QsciScintilla")) {
            currentWidget = (QsciScintilla *)nowWidget;
            currentEditType = "QsciScintilla";
            showPanel();
        }else if (nowWidget->inherits("QComboBox")) {
            QComboBox *cbox = (QComboBox *)nowWidget;

            //只有当下拉选择框处于编辑模式才可以输入
            if (cbox->isEditable()) {
                currentLineEdit = cbox->lineEdit() ;
                currentEditType = "QLineEdit";
                showPanel();
            }
        } else if (nowWidget->inherits("QSpinBox") ||
                   nowWidget->inherits("QDoubleSpinBox") ||
                   nowWidget->inherits("QDateEdit") ||
                   nowWidget->inherits("QTimeEdit") ||
                   nowWidget->inherits("QDateTimeEdit")) {
            currentWidget = nowWidget;
            currentEditType = "QWidget";
        } else {
            //需要将输入法切换到最初的原始状态--小写,同时将之前的对象指针置为零
            currentWidget = 0;
            currentLineEdit = 0;
            currentTextEdit = 0;
            currentPlain = 0;
            currentBrowser = 0;
            currentEditType = "";
            currentType = "english";
            changeType(currentType);
            this->setVisible(false);
        }

        //根据用户选择的输入法位置设置-居中显示-底部填充-显示在输入框正下方
        if (currentPosition == "center") {
            QPoint pos = QPoint(deskWidth / 2 - frmWidth / 2, deskHeight / 2 - frmHeight / 2);
            this->setGeometry(pos.x(), pos.y(), frmWidth, frmHeight);
        } else if (currentPosition == "bottom") {
            this->setGeometry(0, deskHeight - frmHeight, deskWidth, frmHeight);
        } else if (currentPosition == "control") {
            QRect rect = nowWidget->rect();
            QPoint pos = QPoint(rect.left(), rect.bottom() + 2);
            pos = nowWidget->mapToGlobal(pos);

            int x = pos.x();
            int y = pos.y();

            if (pos.x() + frmWidth > deskWidth) {
                x = deskWidth - frmWidth;
            }

            if (pos.y() + frmHeight > deskHeight) {
                y = pos.y() - frmHeight - rect.height() - 2;
            }

            this->setGeometry(x, y, frmWidth, frmHeight);
        }
    }
}

void InputChinese::changeType(QString type)
{
    if (type == "english") {
        this->btnType->setText("En");
        this->widgetTop->setEnabled(false);
        changeChar(false);
    } else if (type == "chinese") {
        this->btnType->setText("中");
        this->widgetTop->setEnabled(true);
        changeChar(false);
    } else if (type == "handwrite") {
        this->btnType->setText("手");
        this->widgetTop->setEnabled(false);
        changeChar(false);
    } else if (type == "char") {
        this->btnType->setText("符");
        this->widgetTop->setEnabled(false);
        changeChar(true);
    }

    //每次切换到模式,都要执行清空之前中文模式下的信息
    clearChinese();
    this->labPY->setText("");
}

void InputChinese::changeLetter(bool isUpper)
{
    QList<QPushButton *> btn = this->findChildren<QPushButton *>();

    foreach (QPushButton *b, btn) {
        if (b->property("btnLetter").toBool()) {
            if (isUpper) {
                b->setText(b->text().toUpper());
            } else {
                b->setText(b->text().toLower());
            }
        }
    }
}

void InputChinese::changeChar(bool isChar)
{
    if (isChar) {
        this->btn1->setText("!");
        this->btn2->setText("@");
        this->btn3->setText("#");
        this->btn4->setText("$");
        this->btn5->setText("%");
        this->btn6->setText("^");
        this->btn7->setText("&&");
        this->btn8->setText("*");
        this->btn9->setText("(");
        this->btn0->setText(")");

        this->btnq->setText("~");
        this->btnw->setText("`");
        this->btne->setText("-");
        this->btnr->setText("_");
        this->btnt->setText("+");
        this->btny->setText("=");
        this->btnu->setText("{");
        this->btni->setText("}");
        this->btno->setText("[");
        this->btnp->setText("]");

        this->btna->setText("|");
        this->btns->setText("\\");
        this->btnd->setText(":");
        this->btnf->setText(";");
        this->btng->setText("\"");
        this->btnh->setText("'");
        this->btnj->setText("<");
        this->btnk->setText(">");
        this->btnl->setText("?");

        this->btnz->setText(",");
        this->btnx->setText("/");
        this->btnc->setText("：");
        this->btnv->setText("，");
        this->btnb->setText("。");
        this->btnn->setText("？");
        this->btnm->setText("；");
    } else {
        this->btn1->setText("1");
        this->btn2->setText("2");
        this->btn3->setText("3");
        this->btn4->setText("4");
        this->btn5->setText("5");
        this->btn6->setText("6");
        this->btn7->setText("7");
        this->btn8->setText("8");
        this->btn9->setText("9");
        this->btn0->setText("0");

        this->btnq->setText("q");
        this->btnw->setText("w");
        this->btne->setText("e");
        this->btnr->setText("r");
        this->btnt->setText("t");
        this->btny->setText("y");
        this->btnu->setText("u");
        this->btni->setText("i");
        this->btno->setText("o");
        this->btnp->setText("p");

        this->btna->setText("a");
        this->btns->setText("s");
        this->btnd->setText("d");
        this->btnf->setText("f");
        this->btng->setText("g");
        this->btnh->setText("h");
        this->btnj->setText("j");
        this->btnk->setText("k");
        this->btnl->setText("l");

        this->btnz->setText("z");
        this->btnx->setText("x");
        this->btnc->setText("c");
        this->btnv->setText("v");
        this->btnb->setText("b");
        this->btnn->setText("n");
        this->btnm->setText("m");
    }
}

void InputChinese::selectChinese()
{
    clearChinese();
    QSqlQuery query(QSqlDatabase::database("py"));
    QString currentPY = this->labPY->text();
    QString sql = QString("select word from pinyin where pinyin='%1'").arg(currentPY);
    query.exec(sql);

    //逐个将查询到的字词加入汉字队列
    while (query.next()) {
        QString result = query.value(0).toString();
        QStringList text = result.split(" ");

        foreach (QString txt, text) {
            if (txt.length() > 0) {
                allPY.append(txt);
                currentPY_count++;
            }
        }
    }

    showChinese();
}

void InputChinese::showChinese()
{
    //每个版面最多显示10个汉字
    int count = 0;
    currentPY.clear();

    for (int i = 0; i < 10; i++) {
        labCh.at(i)->setText("");
    }

    for (int i = currentPY_index; i < currentPY_count; i++) {
        if (count == 10) {
            break;
        }

        QString txt = QString("%1.%2").arg(count).arg(allPY[currentPY_index]);
        currentPY.append(allPY[currentPY_index]);
        labCh.at(count)->setText(txt);
        count++;
        currentPY_index++;
    }

    qDebug() << "currentPY_index:" << currentPY_index << "currentPY_count:" << currentPY_count;
}

void InputChinese::btnClicked()
{
    qDebug() << currentEditType;
    //如果当前焦点控件类型为空,则返回不需要继续处理
    if (currentEditType == "") {
        return;
    }

    QString labText = this->labPY->text();
    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();

    if (objectName == "btnType") {
        if (currentType == "english") {
            currentType = "chinese";
        } else if (currentType == "chinese") {
            currentType = "handwrite";
        } else if (currentType == "handwrite") {
            currentType = "char";
        } else if (currentType == "char") {
            currentType = "english";
        }

        changeType(currentType);

        upper = false;
        changeLetter(upper);
    } else if (objectName == "btnUpper") {
        //大小写模式都要切换到英文状态
        currentType = "english";
        changeType(currentType);

        upper = !upper;
        changeLetter(upper);
    } else if (objectName == "btnDelete") {
        //如果当前是中文模式,则删除对应拼音,删除完拼音之后再删除对应文本输入框的内容
        if (currentType == "chinese") {
            QString txt = labText;
            int len = txt.length();

            if (len > 0) {
                this->labPY->setText(txt.left(len - 1));
                selectChinese();
            } else {
                deleteValue();
            }
        } else {
            deleteValue();
        }
    } else if (objectName == "btnPre") {
        if (currentPY_index >= 20) {
            //每次最多显示10个汉字,所以每次向前的时候索引要减20
            if (currentPY_index % 10 == 0) {
                currentPY_index -= 20;
            } else {
                currentPY_index = currentPY_count - (currentPY_count % 10) - 10;
            }
        } else {
            currentPY_index = 0;
        }

        showChinese();
    } else if (objectName == "btnNext") {
        if (currentPY_index < currentPY_count - 1) {
            showChinese();
        }
    } else if (objectName == "btnClose") {
        this->setVisible(false);
        clearChinese();
        this->labPY->setText("");
    } else if (objectName == "btnSpace") {
        if (currentType == "chinese" && labText != "") {
            setChinese(0);
        } else {
            insertValue(" ");
        }
    } else if (objectName == "btnEnter") {
        QWidget *widget;
        widget = 0;

        if (currentEditType == "QLineEdit") {
            widget = currentLineEdit;
        } else if (currentEditType == "QTextEdit") {
            widget = currentTextEdit;
        } else if (currentEditType == "QPlainTextEdit") {
            widget = currentPlain;
        } else if (currentEditType == "QTextBrowser") {
            widget = currentBrowser;
        } else if (currentEditType == "QWidget" || currentEditType == "QsciScintilla") {
            widget = currentWidget;
        }


        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QApplication::sendEvent(widget, &keyPress);
        this->setVisible(false);
    } else {
        QString value = btn->text();
        qDebug() << "Button Value" << value;
        //如果是&按钮，因为对应&被过滤,所以真实的text为去除前面一个&字符
        if (value == "&&") {
            value = "&";
        }

        //当前不是中文模式,则单击按钮对应text为传递参数
        if (currentType != "chinese") {
            insertValue(value);
        } else {
            //中文模式下,不允许输入特殊字符,单击对应数字按键取得当前索引的汉字
            if (btn->property("btnOther").toBool()) {
                if (labText.length() == 0) {
                    insertValue(value);
                }
            } else if (btn->property("btnNum").toBool()) {
                if (labText.length() == 0) {
                    insertValue(value);
                } else if (objectName == "btn0") {
                    setChinese(0);
                } else if (objectName == "btn1") {
                    setChinese(1);
                } else if (objectName == "btn2") {
                    setChinese(2);
                } else if (objectName == "btn3") {
                    setChinese(3);
                } else if (objectName == "btn4") {
                    setChinese(4);
                } else if (objectName == "btn5") {
                    setChinese(5);
                } else if (objectName == "btn6") {
                    setChinese(6);
                } else if (objectName == "btn7") {
                    setChinese(7);
                } else if (objectName == "btn8") {
                    setChinese(8);
                } else if (objectName == "btn9") {
                    setChinese(9);
                }
            } else if (btn->property("btnLetter").toBool())
            {
                qDebug() << labText + value;
                this->labPY->setText(labText + value);
                selectChinese();
            }
        }
    }
}

void InputChinese::insertValue(QString value)
{
    if (currentEditType == "QLineEdit") {
        currentLineEdit->insert(value);
    } else if (currentEditType == "QTextEdit") {
        currentTextEdit->insertPlainText(value);
    } else if (currentEditType == "QPlainTextEdit") {
        currentPlain->insertPlainText(value);
    } else if (currentEditType == "QTextBrowser") {
        currentBrowser->insertPlainText(value);
    } else if (currentEditType == "QWidget") {
        QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(value));
        QApplication::sendEvent(currentWidget, &keyPress);
    }
}

void InputChinese::deleteValue()
{
    if (currentEditType == "QLineEdit") {
        currentLineEdit->backspace();
    } else if (currentEditType == "QTextEdit") {
        //获取当前QTextEdit光标,如果光标有选中,则移除选中字符,否则删除光标前一个字符
        QTextCursor cursor = currentTextEdit->textCursor();

        if (cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType == "QPlainTextEdit") {
        //获取当前QTextEdit光标,如果光标有选中,则移除选中字符,否则删除光标前一个字符
        QTextCursor cursor = currentPlain->textCursor();

        if (cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType == "QTextBrowser") {
        //获取当前QTextEdit光标,如果光标有选中,则移除选中字符,否则删除光标前一个字符
        QTextCursor cursor = currentBrowser->textCursor();

        if (cursor.hasSelection()) {
            cursor.removeSelectedText();
        } else {
            cursor.deletePreviousChar();
        }
    } else if (currentEditType == "QWidget") {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier, QString());
        QApplication::sendEvent(currentWidget, &keyPress);
    }
}

void InputChinese::setChinese(int index)
{
    int count = currentPY.count();
    if (count > index) {
        insertValue(currentPY[index]);
        //添加完一个汉字后,清空当前汉字信息,等待重新输入
        clearChinese();
        this->labPY->setText("");
    }
}

void InputChinese::clearChinese()
{
    //清空汉字,重置索引
    for (int i = 0; i < 10; i++) {
        labCh.at(i)->setText("");
    }

    allPY.clear();
    currentPY.clear();
    currentPY_index = 0;
    currentPY_count = 0;
}

void InputChinese::changeSize()
{
    QFont btnFont(font().family(), btnFontSize);
    QFont labFont(font().family(), labFontSize);

    QList<QPushButton *> btns = this->widgetMain->findChildren<QPushButton *>();

    foreach (QPushButton *btn, btns) {
        btn->setFont(btnFont);
    }

    QList<QLabel *> labs = this->widgetTop->findChildren<QLabel *>();

    foreach (QLabel *lab, labs) {
        lab->setFont(labFont);
    }

    this->btnPre->setFont(btnFont);
    this->btnNext->setFont(btnFont);

    this->setMinimumSize(frmWidth, frmHeight);

    QSize icoSize(icoWidth, icoHeight);
    QSize icoSize2(icoWidth - 5, icoHeight - 5);
    this->btnDelete->setIconSize(icoSize);
    this->btnSpace->setIconSize(icoSize);
    this->btnClose->setIconSize(icoSize);
    this->btnType->setIconSize(icoSize);
    this->btnEnter->setIconSize(icoSize2);
    this->btnUpper->setIconSize(icoSize2);

    this->widgetMain->layout()->setSpacing(spacing);
    this->widgetPreNext->layout()->setSpacing(spacing);
    this->widgetTop->setFixedHeight(topHeight);
}

void InputChinese::changeStyle()
{
    QString icoType = "white";

    if (currentStyle == "blue") {
        changeStyle("#377FC9", "#F3F3F3", "#4189D3", "#386487", "#F3F3F3", "#386487", "#F3F3F3");
    } else if (currentStyle == "gray") {
        changeStyle("#989898", "#FFFFFF", "#A2A2A2", "#333333", "#F3F3F3", "#2E2E2E", "#F3F3F3");
    } else if (currentStyle == "black") {
        changeStyle("#191919", "#F3F3F3", "#313131", "#4189D3", "#F3F3F3", "#F95717", "#F3F3F3");
    } else if (currentStyle == "brown") {
        changeStyle("#667481", "#F3F3F3", "#566373", "#4189D3", "#F3F3F3", "#4189D3", "#F3F3F3");
    } else if (currentStyle == "silvery") {
        changeStyle("#868690", "#000002", "#C3C2C7", "#393939", "#EFEFEF", "#393939", "#EFEFEF");
        icoType = "black";
    }

    QString ico_upper = QString(":/images/upper_%1.png").arg(icoType);
    QString ico_delete = QString(":/images/delete_%1.png").arg(icoType);
    QString ico_enter = QString(":/images/enter_%1.png").arg(icoType);
    QString ico_close = QString(":/images/close_%1.png").arg(icoType);
    QString ico_space = QString(":/images/space_%1.png").arg(icoType);

    this->btnUpper->setIcon(QIcon(ico_upper));
    this->btnClose->setIcon(QIcon(ico_close));
    this->btnDelete->setIcon(QIcon(ico_delete));
    this->btnSpace->setIcon(QIcon(ico_space));
    this->btnEnter->setIcon(QIcon(ico_enter));
}

void InputChinese::changeStyle(QString mainBackgroundColor, QString mainTextColor, QString btnBackgroundColor,
                              QString btnHoveColor, QString btnHoveTextColor, QString labHoveColor, QString labHoveTextColor)
{
    QStringList qss;
    qss.append(QString("QWidget#widgetMain,QWidget#widgetTop{background-color:%1;}").arg(mainBackgroundColor));
    qss.append(QString("QPushButton{border-width:0px;border-radius:3px;color:%1;}").arg(mainTextColor));
    qss.append(QString("QPushButton{background-color:%1;}").arg(btnBackgroundColor));
    qss.append(QString("QPushButton:pressed,QPushButton:hover{background-color:%1;color:%2;}").arg(btnHoveColor).arg(btnHoveTextColor));
    qss.append(QString("QLabel{border-width:0px;border-radius:3px;color:%1;background:none;}").arg(mainTextColor));
    qss.append(QString("QLabel:pressed,QLabel:hover{background-color:%1;color:%2;}").arg(labHoveColor).arg(labHoveTextColor));
    this->setStyleSheet(qss.join(""));
}

void InputChinese::initFormbyCode()
{
    widgetMain = new QWidget();
    widgetTop = new QWidget();
    widgetPreNext = new QWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *toptHBLayout = new QHBoxLayout();
    QHBoxLayout *topWHBLayout = new QHBoxLayout();
    QHBoxLayout *topPHBLayout = new QHBoxLayout();

    QVBoxLayout *mainkeyLayout = new QVBoxLayout();
    QHBoxLayout *key1HBLayout = new QHBoxLayout();
    QHBoxLayout *key2HBLayout = new QHBoxLayout();
    QHBoxLayout *key3HBLayout = new QHBoxLayout();
    QHBoxLayout *key4HBLayout = new QHBoxLayout();


    labCh0 = new QLabel();
    labCh1 = new QLabel();
    labCh2 = new QLabel();
    labCh3 = new QLabel();
    labCh4 = new QLabel();
    labCh5 = new QLabel();
    labCh6 = new QLabel();
    labCh7 = new QLabel();
    labCh8 = new QLabel();
    labCh9 = new QLabel();
    labPY  = new QLabel();

    topWHBLayout->addWidget(labPY);
    topWHBLayout->addWidget(labCh0);
    topWHBLayout->addWidget(labCh1);
    topWHBLayout->addWidget(labCh2);
    topWHBLayout->addWidget(labCh3);
    topWHBLayout->addWidget(labCh4);
    topWHBLayout->addWidget(labCh5);
    topWHBLayout->addWidget(labCh6);
    topWHBLayout->addWidget(labCh7);
    topWHBLayout->addWidget(labCh8);
    topWHBLayout->addWidget(labCh9);


    btnDot    = new QPushButton();
    btnSpace  = new QPushButton();
    btnEnter  = new QPushButton();
    btnDelete = new QPushButton();
    btnUpper  = new QPushButton();
    btnClose  = new QPushButton();
    btnType   = new QPushButton();
    btnNext   = new QPushButton();
    btnPre    = new QPushButton();
    btn0      = new QPushButton();
    btn1      = new QPushButton();
    btn2      = new QPushButton();
    btn3      = new QPushButton();
    btn4      = new QPushButton();
    btn5      = new QPushButton();
    btn6      = new QPushButton();
    btn7      = new QPushButton();
    btn8      = new QPushButton();
    btn9      = new QPushButton();
    btna      = new QPushButton();
    btnb      = new QPushButton();
    btnc      = new QPushButton();
    btnd      = new QPushButton();
    btne      = new QPushButton();
    btnf      = new QPushButton();
    btng      = new QPushButton();
    btnh      = new QPushButton();
    btni      = new QPushButton();
    btnj      = new QPushButton();
    btnk      = new QPushButton();
    btnl      = new QPushButton();
    btnm      = new QPushButton();
    btnn      = new QPushButton();
    btno      = new QPushButton();
    btnp      = new QPushButton();
    btnq      = new QPushButton();
    btnr      = new QPushButton();
    btns      = new QPushButton();
    btnt      = new QPushButton();
    btnu      = new QPushButton();
    btnv      = new QPushButton();
    btnw      = new QPushButton();
    btnx      = new QPushButton();
    btny      = new QPushButton();
    btnz      = new QPushButton();

    topPHBLayout->addWidget(btnPre);
    topPHBLayout->addWidget(btnNext);

    btnDot->setObjectName("btnDot");
    btnSpace->setObjectName("btnSpace");
    btnEnter->setObjectName("btnEnter");
    btnDelete->setObjectName("btnDelete");
    btnUpper->setObjectName("btnUpper");
    btnClose->setObjectName("btnClose");
    btnType->setObjectName("btnType");
    btnNext->setObjectName("btnNext");
    btnPre->setObjectName("btnPre");

    btn0->setObjectName("btn0");
    btn1->setObjectName("btn1");
    btn2->setObjectName("btn2");
    btn3->setObjectName("btn3");
    btn4->setObjectName("btn4");
    btn5->setObjectName("btn5");
    btn6->setObjectName("btn6");
    btn7->setObjectName("btn7");
    btn8->setObjectName("btn8");
    btn9->setObjectName("btn9");
    btna->setObjectName("btna");
    btnb->setObjectName("btnb");
    btnc->setObjectName("btnc");
    btnd->setObjectName("btnd");
    btne->setObjectName("btne");
    btnf->setObjectName("btnf");
    btng->setObjectName("btng");
    btnh->setObjectName("btnh");
    btni->setObjectName("btni");
    btnj->setObjectName("btnj");
    btnk->setObjectName("btnk");
    btnl->setObjectName("btnl");
    btnm->setObjectName("btnm");
    btnn->setObjectName("btnn");
    btno->setObjectName("btno");
    btnp->setObjectName("btnp");
    btnq->setObjectName("btnq");
    btnr->setObjectName("btnr");
    btns->setObjectName("btns");
    btnt->setObjectName("btnt");
    btnu->setObjectName("btnu");
    btnv->setObjectName("btnv");
    btnw->setObjectName("btnw");
    btnx->setObjectName("btnx");
    btny->setObjectName("btny");
    btnz->setObjectName("btnz");

    labCh0->setObjectName("labCh0");
    labCh1->setObjectName("labCh1");
    labCh2->setObjectName("labCh2");
    labCh3->setObjectName("labCh3");
    labCh4->setObjectName("labCh4");
    labCh5->setObjectName("labCh5");
    labCh6->setObjectName("labCh6");
    labCh7->setObjectName("labCh7");
    labCh8->setObjectName("labCh8");
    labCh9->setObjectName("labCh9");
    labPY ->setObjectName("labPY") ;

    key1HBLayout->addWidget(btn1);
    key1HBLayout->addWidget(btn2);
    key1HBLayout->addWidget(btn3);
    key1HBLayout->addWidget(btn4);
    key1HBLayout->addWidget(btn5);
    key1HBLayout->addWidget(btn6);
    key1HBLayout->addWidget(btn7);
    key1HBLayout->addWidget(btn8);
    key1HBLayout->addWidget(btn9);
    key1HBLayout->addWidget(btn0);
    key1HBLayout->addWidget(btnDot);

    key2HBLayout->addWidget(btnq);
    key2HBLayout->addWidget(btnw);
    key2HBLayout->addWidget(btne);
    key2HBLayout->addWidget(btnr);
    key2HBLayout->addWidget(btnt);
    key2HBLayout->addWidget(btny);
    key2HBLayout->addWidget(btnu);
    key2HBLayout->addWidget(btni);
    key2HBLayout->addWidget(btno);
    key2HBLayout->addWidget(btnp);
    key2HBLayout->addWidget(btnDelete);

    key3HBLayout->addWidget(btnUpper);
    key3HBLayout->addWidget(btna);
    key3HBLayout->addWidget(btns);
    key3HBLayout->addWidget(btnd);
    key3HBLayout->addWidget(btnf);
    key3HBLayout->addWidget(btng);
    key3HBLayout->addWidget(btnh);
    key3HBLayout->addWidget(btnj);
    key3HBLayout->addWidget(btnk);
    key3HBLayout->addWidget(btnl);
    key3HBLayout->addWidget(btnSpace);

    key4HBLayout->addWidget(btnClose);
    key4HBLayout->addWidget(btnType);
    key4HBLayout->addWidget(btnz);
    key4HBLayout->addWidget(btnx);
    key4HBLayout->addWidget(btnc);
    key4HBLayout->addWidget(btnv);
    key4HBLayout->addWidget(btnb);
    key4HBLayout->addWidget(btnn);
    key4HBLayout->addWidget(btnm);
    key4HBLayout->addWidget(btnEnter);

    mainkeyLayout->addLayout(key1HBLayout);
    mainkeyLayout->addLayout(key2HBLayout);
    mainkeyLayout->addLayout(key3HBLayout);
    mainkeyLayout->addLayout(key4HBLayout);

    toptHBLayout->addLayout(topWHBLayout);
    toptHBLayout->addWidget(widgetPreNext);
    widgetPreNext->setLayout(topPHBLayout);
    widgetTop->setLayout(toptHBLayout);
    widgetMain->setLayout(mainkeyLayout);
    widgetTop->setObjectName("inputWidgetTop");
    mainLayout->addWidget(widgetTop);
    mainLayout->addWidget(widgetMain);
    setLayout(mainLayout);
}
