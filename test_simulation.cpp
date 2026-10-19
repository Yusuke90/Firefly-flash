#include "simulation.h"

#include <iostream>

int main(){
    simulation sim(5);

    sim.set_coupling_strength(2.0);

    std::cout << "Initial phases:\n";

    for(int i=0;i<sim.get_number_of_fireflies();i++){
        std::cout <<sim.get_firefly(i).get_phase() << '\n';
    }


    const double dt = 0.01;
    const int steps = 10000;  // 100 seconds

    for (int step = 0; step < steps; ++step)
    {
        sim.step(dt);

        // Print progress every simulated second.
        if ((step + 1) % 100 == 0)
        {
            std::cout << "Time: "
                      << (step + 1) * dt << " s\n";

            for (int i = 0; i < sim.get_number_of_fireflies(); i++)
            {
                std::cout << sim.get_firefly(i).get_phase()
                << " ";
            }

            std::cout << "\nOrder paramater R=" << sim.get_order_paramater() << '\n';

            std::cout << "\n\n";
        }
    }


    std::cout << "\nPhases after 100s:\n";

    for(int i=0;i<sim.get_number_of_fireflies();i++){
        std::cout << sim.get_firefly(i).get_phase() << '\n';
    }

    std::cout << "\nOrder paramater R=" << sim.get_order_paramater() << '\n';

    return 0;
}