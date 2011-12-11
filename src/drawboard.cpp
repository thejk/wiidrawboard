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

    painter.eraseRect(buffer.rect());

    painter.setBrush(QColor(255, 0, 0, 128));
    painter.setPen(QColor(255, 0, 0, 255));

    update();
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

void Drawboard::draw(QPoint p1, QPoint p2)
{
    painter.drawLine(p1, p2);
    update(std::min(p1.x(), p2.x()) - 1,
           std::min(p1.y(), p2.y()) - 1,
           std::max(p1.x(), p2.x()) + 1,
           std::max(p1.y(), p2.y()) + 1);
}

void Drawboard::mousePressEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        QPoint pos = scalePoint(event->pos());
        draw(pos, pos);
        lastDraw = pos;
    }
}

void Drawboard::mouseMoveEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        QPoint pos = scalePoint(event->pos());
        draw(lastDraw, pos);
        lastDraw = pos;
    }
}

void Drawboard::mouseReleaseEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        QPoint pos = scalePoint(event->pos());
        draw(lastDraw, pos);
    }
}

