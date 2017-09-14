#include "asciscintilla.h"
#include "qsciscintilla.h"
#include "qscidocument.h"
#include "qscilexercpp.h"
#include "qsciapis.h"

#include <QVBoxLayout>
#include "common/basedefine.h"
#include <QDebug>
#include <QFont>
#include <QFontDatabase>


AsciScintilla::AsciScintilla(QString textMode, QWidget *parent)
    : QFrame(parent),fileChangeAndSave(false)
{
    int fontId = QFontDatabase::addApplicationFont(":/font/webfont");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont totalFont = QFont(fontName);

    textEdit = new QsciScintilla(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(textEdit);
    textEdit->setObjectName("textEdit");
    textEdit->setFont(totalFont);

    connect(textEdit, SIGNAL(textChanged()), this, SLOT(slotTextEditWasModifed()));
    mainLayout->setMargin(0);
    this->setLayout(mainLayout);
    setTextMode(textMode);
}

AsciScintilla::~AsciScintilla()
{

}

void AsciScintilla::setTextMode(QString textMode)
{
    if(textMode == DOCMENT)
    {
        QsciDocument docment;
        textEdit->setDocument(docment);
        textEdit->setUtf8(true);
    }
    else if(textMode == CPP_TEXT)
    {
       setTextModeCpp(this->textEdit);
    }
}

void AsciScintilla::setAutoCompletion()
{
}

void AsciScintilla::setTextModeCpp(QsciScintilla *edit)
{
    QsciLexerCPP *textLexer = new QsciLexerCPP();
    textLexer->setColor(0x4b5cc4, QsciLexerCPP::Keyword);   //设置自定义关键字的颜色为红色
    QsciAPIs *apis = new QsciAPIs(textLexer);
    if(apis->load(QString(":/autocompletion/cpp")))
        qDebug()<<"读取成功";
    else
        qDebug()<<"读取失败";
    apis->prepare();
    //textLexer->set
    textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);   //自动补全所以地方出现的
    textEdit->setAutoCompletionCaseSensitivity(true);   //设置自动补全大小写敏感
    textEdit->setAutoCompletionThreshold(1);
    edit->setLexer(textLexer);

    edit->setSelectionBackgroundColor(0x808080);
    edit->setUtf8(true);
}

void AsciScintilla::slotTextEditWasModifed()
{
    fileChangeAndSave = true;
}

