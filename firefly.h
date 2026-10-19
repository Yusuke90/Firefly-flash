#ifndef FIREFLY_H
#define FIREFLY_H

class Firefly
{
private:
    double phase;
    double natural_frequency;

    double x;
    double y;
public:
    Firefly(double initial_phase,double frequency,double x_position,double y_position);

    void update(double dt); // dtheta/dt

    double get_phase() const;

    double get_natural_frequency() const;

    double get_x() const;

    double get_y() const;

    double get_brightness() const;

    void set_phase(double new_phase);
};

#endif // FIREFLY_H
