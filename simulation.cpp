
#include "simulation.h"

#include <cmath>
#include <random>
#include <stdexcept>

simulation::simulation(int numberOfFireflies)
    : coupling_strength(1.0)
{
    if (numberOfFireflies <= 0)
        throw std::invalid_argument(
            "Number of fireflies must be positive.");

    std::mt19937 generator(std::random_device{}());

    const double twoPi = 2.0 * std::acos(-1.0);

    std::uniform_real_distribution<double> phaseDistribution(
        0.0, twoPi);

    std::uniform_real_distribution<double> frequencyDistribution(
        0.8, 1.5);

    std::uniform_real_distribution<double> positionDistribution(
        0.0, 500.0);

    for (int i = 0; i < numberOfFireflies; ++i)
    {
        fireflies.emplace_back(
            phaseDistribution(generator),
            frequencyDistribution(generator),
            positionDistribution(generator),
            positionDistribution(generator)
            );
    }
}

void simulation::step(double dt)
{
    if (dt < 0.0)
        throw std::invalid_argument("dt cannot be negative.");

    const int N = static_cast<int>(fireflies.size());

    std::vector<double> oldPhases(N);
    std::vector<double> newPhases(N);

    // First: capture the entire current state.
    for (int i = 0; i < N; ++i)
        oldPhases[i] = fireflies[i].get_phase();

    // Second: calculate every next phase from the old state.
    for (int i = 0; i < N; ++i)
    {
        double phaseRate =
            fireflies[i].get_natural_frequency();

        for (int j = 0; j < N; ++j)
        {
            phaseRate +=
                (coupling_strength / N) *
                std::sin(oldPhases[j] - oldPhases[i]);
        }

        newPhases[i] = oldPhases[i] + phaseRate * dt;
    }

    // Third: apply all phase updates.
    for (int i = 0; i < N; ++i)
        fireflies[i].set_phase(newPhases[i]);
}

void simulation::set_coupling_strength(double K)
{
    coupling_strength = K;
}

double simulation::get_coupling_strength() const
{
    return coupling_strength;
}

int simulation::get_number_of_fireflies() const
{
    return static_cast<int>(fireflies.size());
}

const Firefly& simulation::get_firefly(int index) const
{
    return fireflies.at(index);
}

double simulation::get_order_paramater() const{
    double sum_cos= 0.0;
    double sum_sin= 0.0;

    const int N=static_cast<int>(fireflies.size());

    for(const Firefly& firefly: fireflies){
        const double phase= firefly.get_phase();

        sum_cos+=std::cos(phase);
        sum_sin+=std::sin(phase);
        }

    sum_cos/=N;
    sum_sin/=N;

    return std::sqrt(sum_cos * sum_cos + sum_sin * sum_sin);
}

std::vector<double> simulation::get_phase() const
{
    std::vector<double> phases;

    for (const Firefly& f : fireflies)
    {
        phases.push_back(f.get_phase());
    }

    return phases;
}

void simulation::set_phases(
    const std::vector<double>& phases)
{
    if (phases.size() != fireflies.size())
    {
        throw std::invalid_argument(
            "Phase count does not match firefly count");
    }

    for (std::size_t i = 0; i < fireflies.size(); ++i)
    {
        fireflies[i].set_phase(phases[i]);
    }
}
