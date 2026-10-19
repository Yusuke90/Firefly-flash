#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include "simulation.h"
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <vector>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSignalBlocker>
#include <QString>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class fireflywidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void update_simulation();

    Ui::MainWindow *ui;
    fireflywidget *firefly_widget;
    QTimer timer;
    QElapsedTimer elapsed_timer;
    simulation sim;

    struct SimulationSnapshot
    {
        double time;
        std::vector<double> phases;
    };

    std::vector<SimulationSnapshot> history;


    double simulation_speed = 1.0;
    double simulation_time = 0.0;
    double history_interval = 0.1;
    double history_accumulator = 0.0;

    bool is_paused = false;
    bool is_scrubbing = false;

    QSlider *timeline_slider;
    QSlider *speed_slider;
    QLabel *time_label;
    QLabel *speed_label;
    QPushButton *play_pause_button;
    QLabel *order_parameter_label;
    QProgressBar *order_parameter_bar;

    void record_snapshot();
    void restore_snapshot(int index);
    void update_playback_controls();
};
#endif // MAINWINDOW_H
