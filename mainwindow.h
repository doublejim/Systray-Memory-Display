#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QColorDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QScopedPointer>
#include <QDebug>

#include "aboutdialog.h"
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

signals:
    void sCloseWindow();

public:
    explicit MainWindow(Settings* s, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Settings* settings;

    int m_maskLength = 0;
    bool m_loadingSettings = false;
    AboutDialog aboutDialog;

    void closeEvent(QCloseEvent *event);
    void openColorDialog(QLineEdit *lineEdit);
    void enableRelevantGuiOnly();
    void loadSettingsToGui();
    void saveSettings();
    void somethingChanged();
    void replaceSpaceWithZero(QLineEdit *lineEdit, const QString &arg);

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
    void on_checkBoxAvailableMemory_toggled(bool checked);
    void on_toolButton_clicked();
};

#endif // MAINWINDOW_H
