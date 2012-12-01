#include "formatsdialog.h"

FormatsDialog::FormatsDialog(Project *project, QList<Format*> formats, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormatsDialog)
{
    ui->setupUi(this);

    this->formats = formats;

    this->project = project;

    updateList();

    connect(ui->addPushButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(ui->removePushButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
    connect(ui->removeAllPushButton, SIGNAL(clicked()), this, SLOT(removeAllButtonClicked()));

    errorMessager.setModal(true);
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
        errorMessager.showMessage(tr("Error while loading formatData library"));
        return;
    }

    typedef Format*(*formatDataLoad) (QString& formatFileName, QString* errorMessage);
    formatDataLoad load = (formatDataLoad) formatDataLibrary.resolve("load");

    Format *format = load(name, &errorString);

    FormatValidator::validate(format, errorString);

    if(!errorString.isNull())
    {
        errorMessager.showMessage(errorString);
        return;
    }

    QDir::setCurrent(dirPath);

    QFile luaFile(format->luaInfo["file"]);
    if(!luaFile.exists())
    {
        errorMessager.showMessage(tr("Lua file not found: ") + format->luaInfo["file"]);
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
    QString formatsInfos;

    foreach(Format *format, formats)
    {
        formatsInfos += format->argument["eventID"] + ",";
        formatsInfos += format->argument["argumentID"] + ",";
        formatsInfos += format->argument["eventType"] + ",";
        formatsInfos += format->fileName + ";";
    }

    formatsInfos.chop(1);

    QString errorString = QString::null;

    project->projectParams.visualizationInfo.formatsSettings = formatsInfos;
    project->save(errorString);

    if(!errorString.isNull())
    {
        errorMessager.showMessage(errorString);
        return;
    }

    delete ui;
}

void FormatsDialog::addButtonClicked()
{
    loadFormat();
}

void FormatsDialog::removeButtonClicked()
{
    for(int i = 0; i < formats.size(); i++)
    {
        QString info;
        info += tr("Format name: ") + formats[i]->formatInfo["name"] + " / ";
        info += tr("Argument: ") + formats[i]->formatInfo["argument"] + " / ";
        info += tr("Event type: ") + formats[i]->argument["eventType"];

        if(ui->listWidget->currentItem())
        {
            if(ui->listWidget->currentItem()->text().contains(info))
            {
                delete formats[i];
                formats.removeAt(i);
            }

            delete ui->listWidget->currentItem();
        }
    }
}

void FormatsDialog::removeAllButtonClicked()
{
    foreach(Format *format, formats)
    {
        delete format;
    }

    formats.clear();
    updateList();
}
