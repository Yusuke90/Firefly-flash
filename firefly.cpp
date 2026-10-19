#include "firefly.h"
#include <cmath>


Firefly::Firefly(double initial_phase,double frequency,double x_position,double y_position)
    : phase(initial_phase),natural_frequency(frequency),x(x_position),y(y_position) {}

void Firefly::update(double dt){
    phase+= natural_frequency*dt;
    const double twopi=2.0*std::acos(-1.0);
    phase=std::fmod(phase,twopi);

    while(phase<0.0){
        phase+=twopi;
    }
}

double Firefly::get_phase() const{
    return phase;
}

double Firefly::get_natural_frequency() const{
    return natural_frequency;
}

double Firefly::get_x() const{
    return x;
}

double Firefly::get_y() const{
    return y;
}

double Firefly::get_brightness() const{
    return (1.0 + std::cos(phase)) / 2.0;
}

void Firefly::set_phase(double new_phase){
    const double twopi= 2.0 * std::acos(-1.0);

    phase= std::fmod(new_phase,twopi);

    while(phase<0.0){
        phase+=twopi;
    }
}