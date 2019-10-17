#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

namespace epp
{

struct SimulationClock
{
    using Clock_t     = std::chrono::high_resolution_clock;
    using TimePoint_t = Clock_t::time_point;
    using Duration_t  = TimePoint_t::duration;
    using Int_t       = Duration_t::rep;

    static Int_t getTicksToCatchUp();
    static float getDeltaTime();

    static Int_t update();

    static constexpr Int_t const FPSLimit = 120;
};

} // namespace epp
#endif // CLOCK_H