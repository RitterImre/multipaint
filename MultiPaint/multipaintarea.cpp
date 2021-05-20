#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "multipaintarea.h"
#include "mainwindow.h"

MultiPaintArea::MultiPaintArea(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    painting = false;
}


bool MultiPaintArea::openImage(const QString &fileName) {
    QImage loadedimage;
    if(!loadedimage.load(fileName)) {
        return false;
    }

    QSize newSize = loadedimage.size().expandedTo(size());
    resizeImage(&image, newSize);

    image = loadedimage;
    modified = false;
    update();
    return true;
}

bool MultiPaintArea::saveImage(const QString &fileName, const char *fileFormat) {
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void MultiPaintArea::resizeImage(QImage *image, const QSize &newSize) {
    if(image->size() == newSize) {
        return;
    }

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void MultiPaintArea::printImage() {
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif
}

void MultiPaintArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void MultiPaintArea::resizeEvent(QResizeEvent *event) {
    if(width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void MultiPaintArea::setPenColor(const QColor &newColor) {
    currentPenColor = newColor;
}

void MultiPaintArea::setPenWidth(int newWidth) {
    currentPenWidth = newWidth;
}

void MultiPaintArea::clearImage() {
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void MultiPaintArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        painting = true;
    }
}

void MultiPaintArea::mouseMoveEvent(QMouseEvent *event) {
    if((event->buttons() & Qt::LeftButton) && painting) {
        drawLineTo(event->pos());
    }
}


void MultiPaintArea::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && painting) {
        drawLineTo(event->pos());
        painting = false;
    }
}

void MultiPaintArea::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(currentPenColor, currentPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (currentPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}















