#ifndef SIMULATION_H
#define SIMULATION_H

#include "firefly.h"
#include <vector>

class simulation
{

private:
    std::vector<Firefly> fireflies;
    double coupling_strength;

public:
    explicit simulation(int number_of_fireflies = 40);

    void step(double dt);

    std::vector<double> get_phase() const;

    void set_phases(const std::vector<double>& phases);

    void set_coupling_strength(double k);

    double get_coupling_strength() const;

    int get_number_of_fireflies() const;

    const Firefly& get_firefly(int index) const;

    double get_order_paramater() const;
};

#endif // SIMULATION_H
