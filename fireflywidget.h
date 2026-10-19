#ifndef FIREFLYWIDGET_H
#define FIREFLYWIDGET_H

#include <QWidget>

class QPaintEvent;
class simulation;

class fireflywidget : public QWidget
{
    Q_OBJECT
public:
    explicit fireflywidget(simulation *sim,
                           QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    class simulation *sim;
};

#endif // FIREFLYWIDGET_H
