#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <GGSim/TuplePP.h>
#include <memory>


template<class... MTypes>
struct Initializer
{
    template<class T>
    using MPtr_t = std::unique_ptr<T>;

    using MPack_t = TuplePP<MPtr_t<MTypes>...>;


    Initializer()
    {
        ((mPack.template get<MPtr_t<MTypes>>() = std::make_unique<MTypes>()), ...);
    }

    ~Initializer()
    {
        reversedShutdown<sizeof...(MTypes), MPtr_t<MTypes>...>();
    }

private:
    template<size_t i, class T, class... Rest>
    void reversedShutdown()
    {
        if constexpr (i > 1)
            reversedShutdown<i - 1, Rest...>();
        mPack.template get<T>().reset();
    }

public:
    MPack_t mPack;
};

#endif // INITIALIZER_H