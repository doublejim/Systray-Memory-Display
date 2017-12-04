#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QCloseEvent>
#include <QColorDialog>
#include <QLineEdit>
#include <QDebug>
#include "settings.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

private:
    Ui::SettingsWindow *ui;
    Settings* settings;
    int MASK_LENGTH = 0;
    bool LOADING_SETTINGS = false;
    void closeEvent(QCloseEvent *event);
    void openColorDialog(QLineEdit *lineEdit);
    void enableRelevantGuiOnly();
    void loadSettingsToGui();
    void saveSettings();
    void somethingChanged();
    void replaceSpaceWithZero(QLineEdit *lineEdit, const QString &arg);

public:
    explicit SettingsWindow(Settings* s, QWidget *parent = 0);
    ~SettingsWindow();

private slots:
    void on_btnColor1_clicked();
    void on_btnColor2_clicked();
    void on_btnColor3_clicked();
    void on_btnColor4_clicked();
    void on_btnCancel_clicked();
    void on_btnOK_clicked();
    void on_btnApply_clicked();

    void on_spinBoxInterval_valueChanged(int arg1);
    void on_radioShowProgress_clicked();
    void on_radioShowSolidBackground_clicked();
    void on_checkBoxShowValue_clicked();
    void on_editColorText1_textChanged(const QString &arg1);
    void on_editColorText2_textChanged(const QString &arg1);
    void on_editColorBack1_textChanged(const QString &arg1);
    void on_editColorBack2_textChanged(const QString &arg1);

};

#endif // SETTINGSWINDOW_H
