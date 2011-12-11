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
    void mouseMoveEvent(QMouseEvent* event);

private:
    QPoint scalePoint(QPoint pos) const;
    QRect scaleRect(QRect rect) const;

    QPixmap buffer;
    QPainter painter;
};

#endif /* DRAWBOARD_HPP */
