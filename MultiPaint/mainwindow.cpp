#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "multipaintarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    multiPaintArea = new MultiPaintArea;
    ui->MainWindow::setupUi(this);
    MainWindow::createActions();
    MainWindow::createMenus();
    setCentralWidget(multiPaintArea);
    resize(720, 480);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event) {
    if(maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if(maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty()) {
            multiPaintArea->openImage(fileName);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{

    if(currentFileName.isEmpty()) {
        saveFile("png");
    } else {
        QAction *action = qobject_cast<QAction *>(sender());
        QByteArray fileFormat = action->data().toByteArray();
        multiPaintArea->saveImage(currentFileName, fileFormat.constData());
    }
}

void MainWindow::saveAS() {
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}


bool MainWindow::saveFile(const QByteArray &fileFormat) {
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString windowName;
    QString fileFormats;
    if(fileFormat.isEmpty()) {
        windowName = tr("Save");
        fileFormats = tr("%1 Files (*.%2);; BMP Files (*.bmp);; CUR Files (*.cur);;"
                        "ICO Files (*.ico);; JPEG Files (*.jpg);; PBM Files (*.pbm);; PGM Files (*.pgm);;"
                        "PNG Files (*.png);; PPM Files (*.ppm);; XBM Files (*.xbm);; XPM Files (*.xpm);; All Files(*)");
    } else {
        windowName = tr("Save As");
        fileFormats = tr("%1 Files (*.%2);; All Files(*)");
    }



    QString fileName = QFileDialog::getSaveFileName(this, windowName, initialPath,
                                                    fileFormats
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));
    if(!fileName.isEmpty()) {
        currentFileName = fileName;
        return multiPaintArea->saveImage(fileName, fileFormat.constData());
    } else {
        return false;
    }
}

void MainWindow::on_actionPrint_triggered()
{
    multiPaintArea->printImage();
}

bool MainWindow::maybeSave() {

    if(multiPaintArea->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("MultiPaint"), tr("The image has been modified.\n"
                                            "Do you want to save your changes?"),
                                            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save) {
            return saveFile("png");
        } else if(ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::createActions() {
    foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format.toUpper()));
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(saveAS()));
        saveAsActions.append(action);
    }
}

void MainWindow::createMenus() {
    foreach(QAction *action, saveAsActions) {
        ui->menuSave_as->addAction(action);
    }
}

void MainWindow::on_actionPen_Color_triggered()
{
    QColor newColor = QColorDialog::getColor(multiPaintArea->getPenColor());
    if(newColor.isValid()) {
        multiPaintArea->setPenColor(newColor);
    }
}

void MainWindow::on_actionPen_Width_triggered()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("MultiPaint"), tr("Select pen width: "), multiPaintArea->getPenWidth(), 1, 50, 1, &ok);

    if(ok) {
        multiPaintArea->setPenWidth(newWidth);
    }
}

void MainWindow::on_actionClear_Screen_triggered()
{
    multiPaintArea->clearImage();
}

void MainWindow::on_actionNew_triggered()
{
    if(maybeSave()) {
        multiPaintArea->clearImage();
        currentFileName = "";
    }
}


void MainWindow::on_actionHow_to_use_triggered()
{
    QMessageBox::about(this, tr("About MultiPaint"), tr("<p><b>Draw:</b><br>"
                                                        " - LeftMouse button<br><br>"
                                                        "<b>Pen Color:</b><br>"
                                                        " - Options > Pen Color<br><br>"
                                                        "<b>Pen Size:</b><br>"
                                                        " - Options > Pen Size<br><br>"
                                                        "<b>Clear Screen:</b><br>"
                                                        " - Edit > Clear Screen<br><br>"
                                                        "</p>"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About MultiPaint"), tr("<p>The <b>MultiPaint</b> is a painting program.<br> "
                                                        "Created by <b>YPZFE9</b></p>"));
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}





