#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( Settings* s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow),
    settings(s)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    loadSettingsToGui();
    enableRelevantGuiOnly();
    ui->btnApply->setEnabled(false);

    #if defined(Q_OS_WIN)
      ui->checkBoxAvailableMemory->setEnabled( false);
    #endif

    // Only hexadecimal in color input.
    QString mask = "\\#HHHHHH";
    m_maskLength = mask.length();
    ui->editColorText1->setInputMask(mask);
    ui->editColorText2->setInputMask(mask);
    ui->editColorBack1->setInputMask(mask);
    ui->editColorBack2->setInputMask(mask);

    // Resize editBoxes to contents.
    QFontMetrics fontMetrics( ui->editColorText1->font());
    int width = fontMetrics.width(mask) + 10;
    ui->editColorText1->setMinimumWidth(width);
    ui->editColorText2->setMinimumWidth(width);
    ui->editColorBack1->setMinimumWidth(width);
    ui->editColorBack2->setMinimumWidth(width);
    ui->editColorText1->setMaximumWidth(width);
    ui->editColorText2->setMaximumWidth(width);
    ui->editColorBack1->setMaximumWidth(width);
    ui->editColorBack2->setMaximumWidth(width);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) // on close: just do cancel.
{
    on_btnCancel_clicked();
    event->ignore();
}

void MainWindow::openColorDialog(QLineEdit* lineEdit)
{
    QColorDialog dialog;
    QColor currentColor;

    currentColor.setNamedColor( lineEdit->text());
    QColor color = dialog.getColor(currentColor);
    if (color.isValid())
      lineEdit->setText( color.name());
}

void MainWindow::loadSettingsToGui()
{
    m_loadingSettings = true;
    ui->spinBoxInterval->setValue( settings->value("refreshInterval").toInt());
    ui->checkBoxShowValue->setChecked( settings->value("showValue").toBool());
    ui->radioShowSolidBackground->setChecked( settings->value("showBackground").toBool());
    ui->radioShowProgress->setChecked( settings->value("showBackgroundProgress").toBool());
    ui->editColorText1->setText( settings->value("colorText1").toString());
    ui->editColorText2->setText( settings->value("colorText2").toString());
    ui->editColorBack1->setText( settings->value("colorBack1").toString());
    ui->editColorBack2->setText( settings->value("colorBack2").toString());
    ui->checkBoxAvailableMemory->setChecked( settings->value("availableMemory").toBool());
    m_loadingSettings = false;
}

void MainWindow::saveSettings()
{
    settings->insert("refreshInterval", ui->spinBoxInterval->value());
    settings->insert("showValue", ui->checkBoxShowValue->isChecked());
    settings->insert("showBackground", ui->radioShowSolidBackground->isChecked());
    settings->insert("showBackgroundProgress", ui->radioShowProgress->isChecked());
    settings->insert("colorText1", ui->editColorText1->text());
    settings->insert("colorText2", ui->editColorText2->text());
    settings->insert("colorBack1", ui->editColorBack1->text());
    settings->insert("colorBack2", ui->editColorBack2->text());
    settings->insert("availableMemory", ui->checkBoxAvailableMemory->isChecked());
}

void MainWindow::enableRelevantGuiOnly()
{
    // Disable text color settings if there is no text.
    if (!ui->checkBoxShowValue->isChecked())
    {
        ui->editColorText1->setEnabled(false);
        ui->editColorText2->setEnabled(false);
        ui->labelColor1->setEnabled(false);
        ui->labelColor2->setEnabled(false);
        ui->btnColor1->setEnabled(false);
        ui->btnColor2->setEnabled(false);
    } else {
        ui->editColorText1->setEnabled(true);
        ui->editColorText2->setEnabled(true);
        ui->labelColor1->setEnabled(true);
        ui->labelColor2->setEnabled(true);
        ui->btnColor1->setEnabled(true);
        ui->btnColor2->setEnabled(true);
    }

    // Disable second background color if background is only solid.
    if (ui->radioShowSolidBackground->isChecked())
    {
        ui->editColorBack2->setEnabled(false);
        ui->labelColor4->setEnabled(false);
        ui->btnColor4->setEnabled(false);

        ui->editColorText2->setEnabled(false); // deactivate second text color, if background is solid.
        ui->labelColor2->setEnabled(false);
        ui->btnColor2->setEnabled(false);
    } else {
        ui->editColorBack2->setEnabled(true);
        ui->labelColor4->setEnabled(true);
        ui->btnColor4->setEnabled(true);
    }
}

void MainWindow::on_btnColor1_clicked()
{
    openColorDialog( ui->editColorText1);
}

void MainWindow::on_btnColor2_clicked()
{
    openColorDialog( ui->editColorText2);
}

void MainWindow::on_btnColor3_clicked()
{
    openColorDialog( ui->editColorBack1);
}

void MainWindow::on_btnColor4_clicked()
{
    openColorDialog( ui->editColorBack2);
}

void MainWindow::on_btnCancel_clicked()
{
    settings->undoChanges();
    loadSettingsToGui();
    ui->btnApply->setEnabled(false);
    done(0);
}

void MainWindow::on_btnOK_clicked()
{
    saveSettings();
    settings->applyChangesAndSave();
    done(0);
}

void MainWindow::on_btnApply_clicked()
{
    saveSettings();
    settings->applyChangesAndSave();
    ui->btnApply->setEnabled(false);
}

void MainWindow::somethingChanged()
{
    ui->btnApply->setEnabled(true);
}

void MainWindow::on_spinBoxInterval_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    somethingChanged();
}

void MainWindow::on_radioShowProgress_clicked()
{
    somethingChanged();
    enableRelevantGuiOnly();
}

void MainWindow::on_radioShowSolidBackground_clicked()
{
    somethingChanged();
    enableRelevantGuiOnly();
}

void MainWindow::on_checkBoxShowValue_clicked()
{
    somethingChanged();
    enableRelevantGuiOnly();
}

void MainWindow::replaceSpaceWithZero(QLineEdit* lineEdit, const QString &arg)
{
    if (m_loadingSettings) return;
    if (arg.length() == m_maskLength) return;

    int pos = lineEdit->cursorPosition();

    QString argEdit = arg;
    #if (QT_VERSION > 0x050500)    // Later than 5.5. Maybe even later.
        argEdit.resize( m_maskLength, '0');
    #else
        argEdit += QString( m_maskLength - argEdit.size(), '0');
    #endif

    lineEdit->setText(argEdit);
    lineEdit->setCursorPosition(pos);
}

void MainWindow::on_editColorText1_textChanged(const QString &arg1)
{
    replaceSpaceWithZero(ui->editColorText1, arg1);
    ui->colorExample1->setStyleSheet("background-color:" + arg1 + ";");
    somethingChanged();
}

void MainWindow::on_editColorText2_textChanged(const QString &arg1)
{
    replaceSpaceWithZero(ui->editColorText2, arg1);
    ui->colorExample2->setStyleSheet("background-color:" + arg1 + ";");
    somethingChanged();
}

void MainWindow::on_editColorBack1_textChanged(const QString &arg1)
{
    replaceSpaceWithZero(ui->editColorBack1, arg1);
    ui->colorExample3->setStyleSheet("background-color:" + arg1 + ";");
    somethingChanged();
}

void MainWindow::on_editColorBack2_textChanged(const QString &arg1)
{
    replaceSpaceWithZero(ui->editColorBack2, arg1);
    ui->colorExample4->setStyleSheet("background-color:" + arg1 + ";");
    somethingChanged();
}

void MainWindow::on_checkBoxAvailableMemory_toggled(bool checked)
{
    Q_UNUSED(checked);
    somethingChanged();
}

void MainWindow::on_toolButton_clicked()
{
    aboutDialog.exec();
}
