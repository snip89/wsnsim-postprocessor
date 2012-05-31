#include "viewer.h"

Viewer::Viewer(QWidget *parent)
    : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->setLineWrapMode(QTextEdit::NoWrap);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QSettings settings;
    lineColor = settings.value("Hex visualization/Appearance/Colors_and_Fonts/Cursor_line_color").value<QColor>();

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
