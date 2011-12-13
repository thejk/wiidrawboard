#include "drawboard.hpp"

#include <QtGui>

static const int ERASER_RAD = 5;
static const float ERASER_RAD_2 = ERASER_RAD * ERASER_RAD;

Drawboard::Drawboard(QWidget* parent, tool::Tool initialTool)
    : QWidget(parent), buffer(NULL), painter(NULL),
      currentTool(initialTool), toolVisible(false)
{
    setAttribute(Qt::WA_OpaquePaintEvent);

    setupBuffer();
}

Drawboard::~Drawboard()
{
    delete painter;
    delete buffer;
}

void Drawboard::setupBuffer()
{
    QPixmap* newBuffer = new QPixmap(width(), height());
    QPainter* newPainter = new QPainter(newBuffer);

    newPainter->setBackgroundMode(Qt::OpaqueMode);
    newPainter->setBackground(Qt::white);

    newPainter->setRenderHint(QPainter::Antialiasing);
    newPainter->setRenderHint(QPainter::SmoothPixmapTransform);

    newPainter->eraseRect(newBuffer->rect());
    if (buffer != NULL)
    {
        int w = std::min(buffer->width(), newBuffer->width());
        int h = std::min(buffer->height(), newBuffer->height());
        newPainter->drawPixmap(0, 0, *buffer, 0, 0, w, h);
    }

    newPainter->setBrush(QColor(255, 0, 0, 128));
    newPainter->setPen(QColor(255, 0, 0, 255));

    delete painter;
    delete buffer;
    buffer = newBuffer;
    painter = newPainter;

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

void Drawboard::resizeEvent(QResizeEvent*)
{
    setupBuffer();
}

void Drawboard::paintEvent(QPaintEvent* event)
{
    QPainter paint(this);
    paint.drawPixmap(event->rect(), *buffer, event->rect());
    drawTool(&paint);
}

void Drawboard::draw(QPoint p1, QPoint p2)
{
    switch (currentTool)
    {
    case tool::PEN:
        painter->drawLine(p1, p2);
        update(std::min(p1.x(), p2.x()) - 1,
               std::min(p1.y(), p2.y()) - 1,
               std::max(p1.x(), p2.x()) + 1,
               std::max(p1.y(), p2.y()) + 1);
        break;
    case tool::ERASER:
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(painter->background());
        painter->drawEllipse(p1, ERASER_RAD, ERASER_RAD);
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
            painter->drawPolygon(polys, 4);
            painter->drawEllipse(p2, ERASER_RAD, ERASER_RAD);
        }
        update(std::min(p1.x(), p2.x()) - ERASER_RAD,
               std::min(p1.y(), p2.y()) - ERASER_RAD,
               std::max(p1.x(), p2.x()) + ERASER_RAD,
               std::max(p1.y(), p2.y()) + ERASER_RAD);
        painter->restore();
        break;
    }
}

void Drawboard::mousePressEvent(QMouseEvent* event)
{
    showTool(true);
    if (rect().contains(event->pos()))
    {
        draw(event->pos(), event->pos());
        lastDraw = event->pos();
        updateTool(event->pos());
    }
}

void Drawboard::mouseMoveEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        draw(lastDraw, event->pos());
        lastDraw = event->pos();
        updateTool(event->pos());
    }
}

void Drawboard::mouseReleaseEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        draw(lastDraw, event->pos());
    }
    showTool(false);
}

void Drawboard::setTool(tool::Tool tool)
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
    case tool::PEN:
        break;
    case tool::ERASER:
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
    case tool::PEN:
        break;
    case tool::ERASER:
        painter->save();
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::black);
        painter->drawEllipse(lastToolPos, ERASER_RAD, ERASER_RAD);
        painter->restore();
        break;
    }
}
