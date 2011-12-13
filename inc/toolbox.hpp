#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include <QWidget>

#include "drawboard.hpp"
#include "tool.hpp"

class Toolbox : public QWidget
{
    Q_OBJECT

public:
    Toolbox(QWidget* parent = NULL);

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
    void closed();

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);

    tool::Tool currentTool;
};

#endif /* TOOLBOX_HPP */
