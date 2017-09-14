#ifndef ASCISCINTILLA_H
#define ASCISCINTILLA_H

#include <QFrame>
class QsciScintilla;

class AsciScintilla : public QFrame
{
	Q_OBJECT
public:
    explicit AsciScintilla(QString textMode = "docment", QWidget *parent = 0);
    ~AsciScintilla();

public:
    bool fileChangeAndSave;
    QsciScintilla *textEdit;

private:
    void setTextMode(QString textMode);
    void setAutoCompletion();
    void setTextModeCpp(QsciScintilla *edit);

private slots:
    void slotTextEditWasModifed();
};

#endif
