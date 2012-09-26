#include "viewer.h"

Viewer::Viewer(QString group, QWidget *parent)
    : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // QSettings settings;

    //connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    invisible = false;
}

void Viewer::setLineColor(QColor color)
{
    lineColor = color;
}

void Viewer::setLineFontColor(QColor color)
{
    lineFontColor = color;
}

void Viewer::setInvisible(bool logical)
{
    invisible = logical;
}

Viewer::~Viewer()
{
}

void Viewer::highlightCurrentLine()
{
    if(!invisible)
    {
        disconnect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

        QList<QTextEdit::ExtraSelection> extraSelections;

        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(lineColor);
        selection.format.setForeground(lineFontColor);
        selection.cursor = textCursor();
        selection.cursor.movePosition(QTextCursor::StartOfLine);
        selection.cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

        selection.format.setProperty(QTextFormat::FullWidthSelection, true);

        
        extraSelections.append(selection);

        this->setExtraSelections(extraSelections);

        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    }
}
