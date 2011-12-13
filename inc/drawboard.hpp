#ifndef DRAWBOARD_HPP
#define DRAWBOARD_HPP

#include <QPainter>
#include <QPixmap>
#include <QWidget>

#include "tool.hpp"

class Drawboard : public QWidget
{
    Q_OBJECT

public:
    Drawboard(QWidget* parent = NULL, tool::Tool initialTool = tool::PEN);
    ~Drawboard();

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    tool::Tool tool() const
    {
        return currentTool;
    }

public slots:
    void setTool(tool::Tool tool);

signals:
    void changedTool(tool::Tool newTool);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    void draw(QPoint p1, QPoint p2);

    void showTool(bool show);
    void drawTool(QPainter* painter);
    void updateTool();
    void updateTool(const QPoint& pos);

    void setupBuffer();

    QPixmap* buffer;
    QPainter* painter;
    QPoint lastDraw;
    tool::Tool currentTool;
    QPoint lastToolPos;
    bool toolVisible;
};

#endif /* DRAWBOARD_HPP */
