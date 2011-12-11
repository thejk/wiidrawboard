#ifndef DRAWBOARD_HPP
#define DRAWBOARD_HPP

#include <QPainter>
#include <QPixmap>
#include <QWidget>

class Drawboard : public QWidget
{
    Q_OBJECT

public:
    Drawboard(QWidget* parent = NULL);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
    QPoint scalePoint(QPoint pos) const;
    QRect scaleRect(QRect rect) const;
    void draw(QPoint p1, QPoint p2);

    QPixmap buffer;
    QPainter painter;
    QPoint lastDraw;
};

#endif /* DRAWBOARD_HPP */
