#ifndef DRAWBOARD_HPP
#define DRAWBOARD_HPP

#include <QPainter>
#include <QPixmap>
#include <QWidget>

class Drawboard : public QWidget
{
    Q_OBJECT

public:
    enum Tool
    {
        PEN,
        ERASER,
    };

    Drawboard(QWidget* parent = NULL, Tool initialTool = PEN);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    Tool tool() const
    {
        return currentTool;
    }

public slots:
    void setTool(Tool tool);

signals:
    void changedTool(Tool newTool);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
    QPoint scalePoint(QPoint pos) const;
    QPoint invscalePoint(QPoint pos) const;
    QRect scaleRect(QRect rect) const;
    void draw(QPoint p1, QPoint p2);

    void showTool(bool show);
    void drawTool(QPainter* painter);
    void updateTool();
    void updateTool(const QPoint& pos);

    QPixmap buffer;
    QPainter painter;
    QPoint lastDraw;
    Tool currentTool;
    QPoint lastToolPos;
    bool toolVisible;
};

#endif /* DRAWBOARD_HPP */
