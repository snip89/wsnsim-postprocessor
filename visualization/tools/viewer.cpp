#include "viewer.h"

Viewer::Viewer(QString group, QWidget *parent)
    : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->setLineWrapMode(QTextEdit::NoWrap);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // QSettings settings;

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}

void Viewer::setLineColor(QColor color)
{
    lineColor = color;
}

void Viewer::setLineFontColor(QColor color)
{
    lineFontColor = color;
}

Viewer::~Viewer()
{
}

void Viewer::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(lineColor);
    selection.format.setForeground(lineFontColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    this->setExtraSelections(extraSelections);
}
