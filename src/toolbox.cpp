#include "toolbox.hpp"

#include <QtGui>

Toolbox::Toolbox(QWidget* parent)
    : QWidget(parent), currentTool(tool::PEN)
{
    setFocusPolicy(Qt::StrongFocus);

    emit changedTool(currentTool);
}

QSize Toolbox::sizeHint() const
{
    return QSize(50, 480);
}

QSize Toolbox::minimumSizeHint() const
{
    return QSize(50, 100);
}

void Toolbox::setTool(tool::Tool tool)
{
    if (tool != currentTool)
    {
        currentTool = tool;
        emit changedTool(currentTool);
        update();
    }
}

void Toolbox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    int cx = width() / 2;
    int cy = height() / 2;
    int w = std::min(width(), height());
    w = (w * 3) / 4;
    switch (currentTool)
    {
    case tool::PEN:
        painter.drawLine(cx - w / 2, cy + w / 2, cx + w / 2, cy - w / 2);
        break;
    case tool::ERASER:
        painter.drawEllipse(cx - w / 2, cy - w / 2, w, w);
        break;
    }
}

void Toolbox::mousePressEvent(QMouseEvent*)
{
    if (currentTool == tool::PEN)
    {
        setTool(tool::ERASER);
    }
    else
    {
        setTool(tool::PEN);
    }
}

void Toolbox::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        close();
        return;
    }
    if (event->key() == Qt::Key_E)
    {
        setTool(tool::ERASER);
        return;
    }
    if (event->key() == Qt::Key_P)
    {
        setTool(tool::PEN);
        return;
    }
    QWidget::keyPressEvent(event);
}

void Toolbox::closeEvent(QCloseEvent*)
{
    emit closed();
}
