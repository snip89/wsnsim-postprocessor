#include "viewer.h"

Viewer::Viewer(QString group, QWidget *parent)
    : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->setLineWrapMode(QTextEdit::NoWrap);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QSettings settings;
    lineColor = settings.value(group + "/Appearance/Colors and Fonts/Cursor_line_color").value<QColor>();

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
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
