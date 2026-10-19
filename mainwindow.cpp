
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "fireflywidget.h"

#include <QVBoxLayout>
#include <QElapsedTimer>
#include <algorithm>
#include <QWidget>
#include <QHBoxLayout>
#include <QSignalBlocker>
#include <QString>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sim(40)
    , firefly_widget(nullptr)
{
    ui->setupUi(this);

    QWidget *central = new QWidget(this);

    QVBoxLayout *main_layout = new QVBoxLayout(central);
    QHBoxLayout *timeline_layout = new QHBoxLayout();
    QHBoxLayout *speed_layout = new QHBoxLayout();

    play_pause_button = new QPushButton("Pause", central);
    timeline_slider = new QSlider(Qt::Horizontal, central);
    speed_slider = new QSlider(Qt::Horizontal, central);

    time_label = new QLabel("Time: 0.0 s", central);
    speed_label = new QLabel("Speed: 1.00x", central);

    order_parameter_label =
        new QLabel("Synchronization: 0.00", central);

    order_parameter_bar = new QProgressBar(central);
    order_parameter_bar->setRange(0, 100);
    order_parameter_bar->setValue(0);
    order_parameter_bar->setFormat("%p%");
    order_parameter_bar->setTextVisible(true);

    timeline_slider->setRange(0, 0);
    timeline_slider->setEnabled(false);

    speed_slider->setRange(25, 400);
    speed_slider->setValue(100);

    timeline_layout->addWidget(play_pause_button);
    timeline_layout->addWidget(timeline_slider, 1);
    timeline_layout->addWidget(time_label);

    speed_layout->addWidget(speed_label);
    speed_layout->addWidget(speed_slider, 1);

    firefly_widget = new fireflywidget(&sim, central);

    main_layout->addLayout(timeline_layout);
    main_layout->addLayout(speed_layout);
    main_layout->addWidget(firefly_widget, 1);
    main_layout->addLayout(timeline_layout);
    main_layout->addLayout(speed_layout);
    main_layout->addWidget(order_parameter_label);
    main_layout->addWidget(order_parameter_bar);
    main_layout->addWidget(firefly_widget, 1);

    setCentralWidget(central);

    connect(play_pause_button, &QPushButton::clicked, this, [this]()
            {
                is_paused = !is_paused;

                if (is_paused)
                    play_pause_button->setText("Play");
                else
                    play_pause_button->setText("Pause");

                elapsed_timer.restart();
            });

    connect(speed_slider, &QSlider::valueChanged, this, [this](int value)
            {
                simulation_speed = value / 100.0;

                speed_label->setText(
                    QString("Speed: %1x").arg(simulation_speed, 0, 'f', 2)
                    );
            });

    connect(timeline_slider, &QSlider::sliderPressed, this, [this]()
            {
                is_scrubbing = true;
                is_paused = true;
                play_pause_button->setText("Play");
                timer.stop();
            });

    connect(timeline_slider, &QSlider::sliderReleased, this, [this]()
            {
                restore_snapshot(timeline_slider->value());
                is_scrubbing = false;
                elapsed_timer.restart();
                timer.start();
            });

    connect(timeline_slider, &QSlider::valueChanged, this, [this](int index)
            {
                if (is_scrubbing)
                    restore_snapshot(index);
            });

    connect(&timer, &QTimer::timeout,
            this, &MainWindow::update_simulation);

    timer.setInterval(16);

    elapsed_timer.start();
    timer.start();

    simulation_time = 0.0;
    history_accumulator = 0.0;

    record_snapshot();

    timeline_slider->setRange(0, 0);
    timeline_slider->setValue(0);

    QTimer::singleShot(0, this, [this]()
                       {
                           showMaximized();
                       });
}

void MainWindow::record_snapshot()
{
    history.push_back({
        simulation_time,
        sim.get_phase()
    });
}

void MainWindow::restore_snapshot(int index)
{
    if (index < 0 ||
        index >= static_cast<int>(history.size()))
    {
        return;
    }

    const SimulationSnapshot& snapshot = history[index];

    sim.set_phases(snapshot.phases);
    simulation_time = snapshot.time;

    firefly_widget->update();
    update_playback_controls();
}

void MainWindow::update_playback_controls()
{
    if (history.empty())
        return;

    const double shown_time = simulation_time;

    time_label->setText(
        QString("Time: %1 s").arg(shown_time, 0, 'f', 1)
        );

    double r = sim.get_order_paramater();

    order_parameter_label->setText(
        QString("Synchronization Score: %1")
            .arg(r, 0, 'f', 3)
        );

    order_parameter_bar->setValue(
        static_cast<int>(r * 100.0 + 0.5)
        );
}

void MainWindow::update_simulation()
{
    if (is_paused || is_scrubbing)
        return;

    double dt = elapsed_timer.restart() / 1000.0;
    dt = std::min(dt, 0.05);

    dt *= simulation_speed;

    sim.step(dt);
    simulation_time += dt;

    history_accumulator += dt;

    if (history_accumulator >= history_interval)
    {
        history_accumulator = 0.0;
        record_snapshot();

        QSignalBlocker blocker(timeline_slider);
        timeline_slider->setRange(
            0, static_cast<int>(history.size()) - 1
            );
        timeline_slider->setEnabled(history.size() > 1);
        timeline_slider->setValue(
            static_cast<int>(history.size()) - 1
            );
    }

    update_playback_controls();
    firefly_widget->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
