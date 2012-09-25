/*
 *    Created by Sergey Popov
 *    snip89@mail.ru
 */

#ifndef ABSTRACTTEXTGUISSETTINGS_H
#define ABSTRACTTEXTGUISSETTINGS_H

#include <QWidget>
#include <QObject>

#include "ui_abstracttextguisettings.h"

namespace Ui {
	class AbstractTextGuiSettings;
}

class AbstractTextGuiSettings : public QWidget
{
	Q_OBJECT

public:
	explicit AbstractTextGuiSettings(QWidget *parent = 0);

	virtual ~AbstractTextGuiSettings();
protected:
	Ui::AbstractTextGuiSettings *ui;	
};

#endif // ABSTRACTTEXTGUISSETTINGS_H