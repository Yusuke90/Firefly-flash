#include "fireflywidget.h"
#include "simulation.h"

#include <QPainter>
#include <QColor>
#include <QPointF>
#include <algorithm>
#include <cmath>


fireflywidget::fireflywidget(simulation *sim,
                             QWidget *parent)
    : QWidget(parent), sim(sim)
{
    setMinimumSize(500,400);
}

void fireflywidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(),Qt::black);
    painter.setPen(Qt::NoPen);

    if(!sim){
        return;
    }

    for(int i=0;i<sim->get_number_of_fireflies();i++){
        const Firefly& f=sim->get_firefly(i);


        const double px=f.get_x()*width()/500.0;
        const double py=f.get_y()*height()/500.0;

        const QPointF center(px,py);

        const double brightness = f.get_brightness();

        // Boost dim fireflies while preserving the brightness cycle.
        const double visual_brightness = std::pow(brightness, 0.65);

        const int outerAlpha =
            static_cast<int>(85 * visual_brightness);

        const int middleAlpha =
            static_cast<int>(180 * visual_brightness);

        painter.setBrush(QColor(180,255,80,outerAlpha));
        painter.drawEllipse(center,30,30);

        painter.setBrush(QColor(200,255,100,middleAlpha));
        painter.drawEllipse(center,16,16);

        const int coreIntensity=
            static_cast<int>(100+155*brightness);

        painter.setBrush(QColor(
            coreIntensity,
            coreIntensity,
            180+static_cast<int>(75*brightness)
            ));

        painter.drawEllipse(center,5,5);
    }
}
