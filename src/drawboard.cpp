#include "drawboard.hpp"

#include <QtGui>

Drawboard::Drawboard(QWidget* parent)
    : QWidget(parent), buffer(sizeHint()), painter(&buffer)
{
    setAttribute(Qt::WA_OpaquePaintEvent);

    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setBackground(Qt::white);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.setBrush(QColor(255, 0, 0, 128));
    painter.setPen(QColor(255, 0, 0, 196));
}

QSize Drawboard::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Drawboard::sizeHint() const
{
    return QSize(640, 480);
}

QPoint Drawboard::scalePoint(QPoint pos) const
{
    QPoint ret(pos);
    if (buffer.width() != width())
    {
        const double scale = (double)buffer.width() / (double)width();
        ret.setX(pos.x() * scale);
    }
    if (buffer.height() != height())
    {
        const double scale = (double)buffer.height() / (double)height();
        ret.setY(pos.y() * scale);
    }
    return ret;
}

QRect Drawboard::scaleRect(QRect rect) const
{
    QRect ret(rect);
    if (buffer.width() != width())
    {
        const double scale = (double)buffer.width() / (double)width();
        ret.setLeft(rect.left() * scale);
        ret.setRight(rect.right() * scale);
    }
    if (buffer.height() != height())
    {
        const double scale = (double)buffer.height() / (double)height();
        ret.setTop(rect.top() * scale);
        ret.setBottom(rect.bottom() * scale);
    }
    return ret;
}

void Drawboard::paintEvent(QPaintEvent* event)
{
    QPainter paint(this);
    QRect dst = event->rect(), src(scaleRect(dst));
    paint.setRenderHint(QPainter::SmoothPixmapTransform);
    paint.drawPixmap(dst, buffer, src);
}

void Drawboard::mouseMoveEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        QPoint pos = scalePoint(event->pos());
        painter.drawEllipse(pos, 2, 2);
        update(pos.x() - 2, pos.y() - 2, 4, 4);
    }
}


