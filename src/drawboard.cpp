#include "drawboard.hpp"

#include <QtGui>
#include <iostream>

static const int ERASER_RAD = 5;
static const float ERASER_RAD_2 = ERASER_RAD * ERASER_RAD;

Drawboard::Drawboard(QWidget* parent, Tool initialTool)
    : QWidget(parent), buffer(sizeHint()), painter(&buffer),
      currentTool(initialTool), toolVisible(false)
{
    setFocusPolicy(Qt::StrongFocus);
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

QPoint Drawboard::invscalePoint(QPoint pos) const
{
    QPoint ret(pos);
    if (buffer.width() != width())
    {
        const double scale = (double)width() / (double)buffer.width();
        ret.setX(pos.x() * scale);
    }
    if (buffer.height() != height())
    {
        const double scale = (double)height() / (double)buffer.height();
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
    drawTool(&paint);
}

void Drawboard::draw(QPoint p1, QPoint p2)
{
    switch (currentTool)
    {
    case PEN:
        painter.drawLine(p1, p2);
        update(std::min(p1.x(), p2.x()) - 1,
               std::min(p1.y(), p2.y()) - 1,
               std::max(p1.x(), p2.x()) + 1,
               std::max(p1.y(), p2.y()) + 1);
        break;
    case ERASER:
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(painter.background());
        painter.drawEllipse(p1, ERASER_RAD, ERASER_RAD);
        if (p1 != p2)
        {
            const int dy = p2.y() - p1.y();
            QPoint polys[4];
            int x, y;
            if (dy != 0)
            {
                float k = (float)(p2.x() - p1.x()) / (float)-dy;
                float _x = sqrtf(ERASER_RAD_2 / (1.0f + k * k));
                x = rint(_x);
                y = rint(_x * k);
            }
            else
            {
                x = 0;
                y = ERASER_RAD;
            }
            polys[0].setX(p1.x() - x);
            polys[0].setY(p1.y() - y);
            polys[1].setX(p1.x() + x);
            polys[1].setY(p1.y() + y);
            polys[2].setX(p2.x() + x);
            polys[2].setY(p2.y() + y);
            polys[3].setX(p2.x() - x);
            polys[3].setY(p2.y() - y);
            painter.drawPolygon(polys, 4);
            painter.drawEllipse(p2, ERASER_RAD, ERASER_RAD);
        }
        update(std::min(p1.x(), p2.x()) - ERASER_RAD,
               std::min(p1.y(), p2.y()) - ERASER_RAD,
               std::max(p1.x(), p2.x()) + ERASER_RAD,
               std::max(p1.y(), p2.y()) + ERASER_RAD);
        painter.restore();
        break;
    }
}

void Drawboard::mousePressEvent(QMouseEvent* event)
{
    showTool(true);
    if (rect().contains(event->pos()))
    {
        QPoint pos = scalePoint(event->pos());
        draw(pos, pos);
        lastDraw = pos;
        updateTool(event->pos());
    }
}

void Drawboard::mouseMoveEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        QPoint pos = scalePoint(event->pos());
        draw(lastDraw, pos);
        lastDraw = pos;
        updateTool(event->pos());
    }
}

void Drawboard::mouseReleaseEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        QPoint pos = scalePoint(event->pos());
        draw(lastDraw, pos);
    }
    showTool(false);
}

void Drawboard::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        close();
        return;
    }
    if (event->key() == Qt::Key_E)
    {
        setTool(ERASER);
        return;
    }
    if (event->key() == Qt::Key_P)
    {
        setTool(PEN);
        return;
    }
    QWidget::keyPressEvent(event);
}

void Drawboard::setTool(Tool tool)
{
    if (tool == currentTool)
    {
        return;
    }

    updateTool();
    currentTool = tool;
    updateTool();
    emit changedTool(currentTool);
}

void Drawboard::showTool(bool show)
{
    if (toolVisible != show)
    {
        if (show)
        {
            toolVisible = show;
            updateTool();
        }
        else
        {
            updateTool();
            toolVisible = show;
        }
    }
}

void Drawboard::updateTool()
{
    if (!toolVisible)
    {
        return;
    }

    switch (currentTool)
    {
    case PEN:
        break;
    case ERASER:
        update(lastToolPos.x() - ERASER_RAD, lastToolPos.y() - ERASER_RAD,
               lastToolPos.x() + ERASER_RAD, lastToolPos.y() + ERASER_RAD);
        break;
    }
}

void Drawboard::updateTool(const QPoint& pos)
{
    if (pos != lastToolPos)
    {
        updateTool();
        lastToolPos = pos;
        updateTool();
    }
}

void Drawboard::drawTool(QPainter* painter)
{
    if (!toolVisible)
    {
        return;
    }

    switch (currentTool)
    {
    case PEN:
        break;
    case ERASER:
        QPoint rad(invscalePoint(QPoint(ERASER_RAD, ERASER_RAD)));
        painter->save();
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::black);
        painter->drawEllipse(lastToolPos, rad.x(), rad.y());
        painter->restore();
        break;
    }
}
