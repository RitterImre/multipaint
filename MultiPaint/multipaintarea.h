#ifndef MULTIPAINTAREA_H
#define MULTIPAINTAREA_H

#include <QWidget>
#include <QImage>
#include <QWidget>

class MultiPaintArea : public QWidget {
    Q_OBJECT

public:
    MultiPaintArea(QWidget *parent = 0);
    bool saveImage(const QString &fileName, const char *fileFormat);
    bool openImage(const QString &fileName);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return modified; };
    bool isPainting() const { return painting; };
    QColor getPenColor() const { return currentPenColor; };
    int getPenWidth() const { return currentPenWidth; };

public slots:
    void clearImage();
    void printImage();

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    QImage image;
    bool modified;
    bool painting;
    QColor currentPenColor;
    int currentPenWidth;
    QPoint lastPoint;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

};
#endif // MULTIPAINTAREA_H
