#include <GGSim/Matrix.h>
#include <assert.h>

template<class T, size_t n>
struct Vec : public Matrix<T, 1, n>
{
    static_assert(n, "Cannot create empty vector");

private:
    using Mat_t = Matrix<T, 1, n>;

    using Vec_t = Vec<T, n>;

public:
    Vec() = default;

    Vec(T (&arr)[n])
    {
        for (int i = 0; i < n; ++i)
            this->arr[1][i] = arr[i];
    }


    float length() const
    {
        float len = Mat_t::arr[0] * Mat_t::arr[0];
        for (int i = 1; i < n; ++i)
            len += Mat_t::arr[n] * Mat_t::arr[n];
        return std::sqrt(len);
    }


    Vec_t& normalize()
    {
        float len = length();
        if (len)
            return *this /= len;
        return *this = Mat_t::Zero;
    }

    Vec_t normalized() const { return Vec_t(*this).normalize(); }


    T& x() { return Mat_t::arr[1][0]; }

    T const& x() const { return Mat_t::arr[1][0]; }

    T& y() { return Mat_t::arr[1][1]; }

    T const& y() const { return Mat_t::arr[1][1]; }

    template<size_t s = 0>
    std::enable_if_t<(n >= 3 && (s || !s)), T&> z()
    {
        return Mat_t::arr[1][2];
    }

    template<size_t s = 0>
    std::enable_if_t<(n >= 3 && (s || !s)), T const&> z() const
    {
        return Mat_t::arr[1][2];
    }

    template<size_t s = 0>
    std::enable_if_t<(n >= 4 && (s || !s)), T&> w()
    {
        return Mat_t::arr[1][3];
    }

    template<size_t s = 0>
    std::enable_if_t<(n >= 4 && (s || !s)), T const&> w() const
    {
        return Mat_t::arr[1][3];
    }
};


// Aliases

using Vec3f = Vec<float, 3>;
using Vec3u = Vec<uint32_t, 3>;
using Vec3i = Vec<int32_t, 3>;

using Vec4f = Vec<float, 4>;
using Vec4u = Vec<uint32_t, 4>;
using Vec4i = Vec<int32_t, 4>;