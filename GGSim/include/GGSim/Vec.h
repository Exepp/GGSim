#include <GGSim/Matrix.h>
#include <array>

template<class T, size_t n>
struct Vec : public Matrix<T, 1, n>
{
    static_assert(n, "Cannot create empty vector");

private:
    using Mat_t = Matrix<T, 1, n>;

    using Vec_t = Vec<T, n>;

public:
    Vec() = default;

    template<size_t n2>
    explicit Vec(Vec<T, n2> const& other)
        : Mat_t(other)
    {}

    Vec(Mat_t const& mat)
    {
        for (int i = 0; i < n; ++i)
            Mat_t::arr[0][i] = mat.arr[0][i];
    }

    Vec(std::array<T, n> const& arr)
    {
        for (int i = 0; i < n; ++i)
            Mat_t::arr[0][i] = arr[i];
    }


    Float_t length() const
    {
        Float_t len = Mat_t::arr[0][0] * Mat_t::arr[0][0];
        for (int i = 1; i < n; ++i)
            len += Mat_t::arr[0][i] * Mat_t::arr[0][i];
        return std::sqrt(len);
    }


    Vec_t& normalize()
    {
        Float_t len = length();
        if (len)
            return (Vec_t&)(*this /= len);
        return *this = Mat_t::Zero;
    }

    Vec_t normalized() const { return Vec_t(*this).normalize(); }


    T& x() { return Mat_t::arr[0][0]; }

    T const& x() const { return Mat_t::arr[0][0]; }

    T& y() { return Mat_t::arr[0][1]; }

    T const& y() const { return Mat_t::arr[0][1]; }

    template<size_t s = 0>
    std::enable_if_t<(n >= 3 && (s || !s)), T&> z()
    {
        return Mat_t::arr[0][2];
    }

    template<size_t s = 0>
    std::enable_if_t<(n >= 3 && (s || !s)), T const&> z() const
    {
        return Mat_t::arr[0][2];
    }

    template<size_t s = 0>
    std::enable_if_t<(n >= 4 && (s || !s)), T&> w()
    {
        return Mat_t::arr[0][3];
    }

    template<size_t s = 0>
    std::enable_if_t<(n >= 4 && (s || !s)), T const&> w() const
    {
        return Mat_t::arr[0][3];
    }
};


// Aliases

using Vec3f = Vec<Float_t, 3>;
using Vec3u = Vec<uint32_t, 3>;
using Vec3i = Vec<int32_t, 3>;

using Vec4f = Vec<Float_t, 4>;
using Vec4u = Vec<uint32_t, 4>;
using Vec4i = Vec<int32_t, 4>;


// constants:

static Vec3f const AxisX({ 1, 0, 0 });
static Vec3f const AxisY({ 0, 1, 0 });
static Vec3f const AxisZ({ 0, 0, 1 });

// vec3 math utilities:


inline Vec3f crossProduct(Vec3f const& lhs, Vec3f const& rhs)
{
    return Vec3f({ lhs.y() * rhs.z() - lhs.z() * rhs.y(),
                   rhs.x() * lhs.z() - rhs.z() * lhs.x(),
                   lhs.x() * rhs.y() - lhs.y() * rhs.x() });
}

inline decltype(auto) dotProduct(Vec3f const& lhs, Vec3f const& rhs)
{
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}


inline decltype(auto) lengthOfProjAonB(Vec3f const& a, Vec3f const& b)
{
    return dotProduct(b.normalized(), a);
}

inline Vec3f projectAonB(Vec3f const& a, Vec3f const& b)
{
    Vec<Float_t, 3> bNorm = b.normalized();
    return bNorm *= dotProduct(bNorm, a);
}

inline Float_t angleBetweenVectors(Vec3f const& a, Vec3f const& b)
{
    return atan2(crossProduct(a, b).length(), dotProduct(a, b));
}

inline Float_t signedAngleBetweenVectors(Vec3f const& a, Vec3f const& b, Vec3f const& refNorm)
{
    // angle * sign
    return angleBetweenVectors(a, b) * (dotProduct(crossProduct(refNorm, a), b) < 0 ? -1 : 1);
}