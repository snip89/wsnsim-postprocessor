#include "viewer.h"

Viewer::Viewer(QWidget *parent)
    : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->setLineWrapMode(QTextEdit::NoWrap);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    lineColor = QColor(Qt::yellow).lighter(160);

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}

void Viewer::setLineColor(QColor color)
{
    lineColor = color;
}

Viewer::~Viewer()
{
}

void Viewer::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    this->setExtraSelections(extraSelections);
}
