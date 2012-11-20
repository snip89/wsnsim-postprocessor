#include "formatsdialog.h"

FormatsDialog::FormatsDialog(Project *project, QList<Format*> formats, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormatsDialog)
{
    ui->setupUi(this);

    this->formats = formats;

    this->project = project;

    updateList();

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));

    errorMessager = new QErrorMessage(this);
}

void FormatsDialog::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);

    QMenu menu;
    menu.addAction(tr("add format"));

    QListWidgetItem* temp = ui->listWidget->itemAt(pos);
    if(temp)
    {
        menu.addAction(tr("delete format"));
    }

    if(formats.size() != 0)
    {
        menu.addSeparator();
        menu.addAction(tr("remove all"));
    }

    QAction* selectedItem = menu.exec(globalPos);
    if (selectedItem)
    {
        if(selectedItem->text() == tr("add format"))
        {
            loadFormat();
        }

        else if(selectedItem->text() == tr("delete format"))
        {
            for(int i = 0; i < formats.size(); i++)
            {
                QString info;
                info += tr("Format name: ") + formats[i]->formatInfo["name"] + " / ";
                info += tr("Argument: ") + formats[i]->formatInfo["argument"] + " / ";
                info += tr("Event type: ") + formats[i]->argument["eventType"];

                if(temp->text().contains(info))
                {
                    delete formats[i];
                    formats.removeAt(i);
                }
            }

            delete temp;
        }

        else if(selectedItem->text() == tr("remove all"))
        {
            foreach(Format *format, formats)
            {
                delete format;
            }

            formats.clear();
            updateList();
        }
    }
}

void FormatsDialog::loadFormat()
{
    QString name = "";

    QString dirPath = settings.value("General/Gui/Format_file_dialog_path").toString();

    QFileDialog *fileDialog = new QFileDialog(this, tr("Open format file"), dirPath, tr("XML format files (*xml)"));
    fileDialog->move(settings.value("Hidden/Gui/File_dialog_pos").value<QPoint>());
    fileDialog->resize(settings.value("Hidden/Gui/File_dialog_size").value<QSize>());
    fileDialog->setWindowIcon(QIcon(":/icons/folder"));

    if(fileDialog->exec())
        name = fileDialog->selectedFiles().at(0);

    settings.setValue("Hidden/Gui/File_dialog_pos", fileDialog->pos());
    settings.setValue("Hidden/Gui/File_dialog_size", fileDialog->size());

    delete fileDialog;

    if(name == "")
        return;

    QDir dir;
    QFileInfo finfo = QFileInfo(name);

    dirPath = dir.filePath(name);
    dirPath.chop(finfo.fileName().size());
    settings.setValue("General/Gui/Format_file_dialog_path", dirPath);

    QDir::setCurrent(QApplication::applicationDirPath());

    QString errorString = QString::null;
    QLibrary formatDataLibrary("./formatData");
    if(!formatDataLibrary.load())
    {
        errorMessager->showMessage(tr("Error while loading formatData library"));
        return;
    }

    typedef Format*(*formatDataLoad) (QString& formatFileName, QString* errorMessage);
    formatDataLoad load = (formatDataLoad) formatDataLibrary.resolve("load");

    Format *format = load(name, &errorString);

    FormatValidator::validate(format, errorString);

    if(!errorString.isNull())
    {
        errorMessager->showMessage(errorString);
        return;
    }

    QDir::setCurrent(dirPath);

    QFile luaFile(format->luaInfo["file"]);
    if(!luaFile.exists())
    {
        errorMessager->showMessage(tr("Lua file not found"));
        return;
    }

    QDir::setCurrent(QApplication::applicationDirPath());

    FormatAcceptingDialog *formatAcceptingDialog = new FormatAcceptingDialog(project,
                                                                             format->formatInfo["argument"]);

    formatAcceptingDialog->move(settings.value("Hidden/Gui/Columns_selection_dialog_pos").value<QPoint>());

    if(formatAcceptingDialog->exec())
    {
        settings.setValue("Hidden/Gui/Columns_selection_dialog_pos", formatAcceptingDialog->pos());

        AttrInfo info = formatAcceptingDialog->getArgument();

        format->argument = info;

        // TODO: убивать формат если такой уже есть
        //project->injectFormatSettings();

        formats.append(format);
    }
    else
    {
        settings.setValue("Hidden/Gui/Columns_selection_dialog_pos", formatAcceptingDialog->pos());
        delete format;
        return;
    }

    delete formatAcceptingDialog;

    updateList();
}

QList<Format*> FormatsDialog::getFormats()
{
    return formats;
}

void FormatsDialog::updateList()
{
    ui->listWidget->clear();

    foreach(Format *format, formats)
    {
        QString info;
        info += tr("Format name: ") + format->formatInfo["name"] + " / ";
        info += tr("Argument: ") + format->formatInfo["argument"] + " / ";
        info += tr("Event type: ") + format->argument["eventType"];

        ui->listWidget->addItem(info);
    }
}

FormatsDialog::~FormatsDialog()
{
    delete ui;
    delete errorMessager;
}
