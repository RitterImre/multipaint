#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "multipaintarea.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MultiPaintArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionPrint_triggered();
    void on_actionSave_triggered();
    void on_actionPen_Color_triggered();
    void on_actionPen_Width_triggered();
    void on_actionClear_Screen_triggered();
    void on_actionNew_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionHow_to_use_triggered();
    void saveAS();

private:
    Ui::MainWindow *ui;
    MultiPaintArea *multiPaintArea;
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
    QList<QAction *> saveAsActions;
    QString currentFileName;

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
