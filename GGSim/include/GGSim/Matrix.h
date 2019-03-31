#include <array>
#include <cmath>
#include <cstring>


template<class T, size_t row, size_t col>
class Matrix
{
private:
    using Mat_t = Matrix<T, row, col>;

public:
    Matrix() = default;

    template<size_t row2, size_t col2>
    explicit Matrix(Matrix<T, row2, col2> const& other)
    {
        static constexpr size_t minRow = std::min(row, row2);
        static constexpr size_t minCol = std::min(col, col2);

        for (int i = 0; i < minRow; ++i)
            for (int j = 0; j < minCol; ++j)
                arr[i][j] = other.arr[i][j];
        for (int i = minRow; i < row; ++i)
            for (int j = minCol; j < col; ++j)
                arr[i][j] = 0;
    }

    explicit Matrix(T const& val, bool diagonal)
    {
        if (diagonal)
        {
            for (int i = 0; i < row; ++i)
                for (int j = 0; j < col; ++j)
                    arr[i][j] = (i == j) * val;
        }
        else
            for (int i = 0; i < row; ++i)
                for (int j = 0; j < col; ++j)
                    arr[i][j] = val;
    }

    Matrix(T const (&arr)[row][col])
    {
        std::memcpy(this->arr, arr, row * col);
    }

    decltype(auto) transposed() const
    {
        Matrix<T, col, row> result;
        for (int i = 0; i < col; ++i)
            for (int j = 0; j < row; ++j)
                result[i][j] = arr[j][i];
        return result;
    }

    template<size_t s = 0>
    std::enable_if_t<(row == col && (s || !s)), Mat_t&> transpose()
    {
        *this = transposed();
    }


    T* operator[](size_t i)
    {
        return arr[i];
    }

    T const* operator[](size_t i) const
    {
        return arr[i];
    }

public:
    static Mat_t const Identity;

    static Mat_t const Zero;


    T arr[row][col];
};


template<class T, size_t row, size_t col>
Matrix<T, row, col> const Matrix<T, row, col>::Identity(1, true);

template<class T, size_t row, size_t col>
Matrix<T, row, col> const Matrix<T, row, col>::Zero;


// Aliases

using Mat44f = Matrix<float, 4, 4>;
using Mat44u = Matrix<uint32_t, 4, 4>;
using Mat44i = Matrix<int32_t, 4, 4>;

using Mat33f = Matrix<float, 3, 3>;
using Mat33u = Matrix<uint32_t, 3, 3>;
using Mat33i = Matrix<int32_t, 3, 3>;


// Operators:

// matrix matrix multiplication:

template<class T, size_t row1, size_t col1, size_t col2>
Matrix<T, row1, col2> operator*(Matrix<T, row1, col1> const& lhs, Matrix<T, col1, col2> const& rhs)
{
    Matrix<T, row1, col2> result;
    for (int i = 0; i < row1; ++i)
        for (int j = 0; j < col2; ++j)
        {
            T val = 0;
            for (int k = 0; k < col1; ++k)
                val += lhs[i][k] * rhs[k][j];
            result[i][j] = val;
        }
    return result;
}

template<class T, size_t row1, size_t col1, size_t col2>
Matrix<T, row1, col2> operator*=(Matrix<T, row1, col1>& lhs, Matrix<T, col1, col2> const& rhs)
{
    return lhs = lhs * rhs;
}


// matrix scalar multiplication/division:


template<class T, size_t row, size_t col>
Matrix<T, row, col> operator*(Matrix<T, row, col> const& mat, T const& scalar)
{
    Matrix<T, row, col> result;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            result[i][j] = mat[i][j] * scalar;
    return result;
}

template<class T, size_t row, size_t col>
Matrix<T, row, col> operator*(T const& scalar, Matrix<T, row, col> const& mat)
{
    return mat * scalar;
}

template<class T, size_t row, size_t col>
Matrix<T, row, col>& operator*=(Matrix<T, row, col> const& mat, T const& scalar)
{
    return mat = mat * scalar;
}

template<class T, size_t row, size_t col>
inline Matrix<T, row, col> operator/(Matrix<T, row, col> const& mat, T scalar)
{
    if (scalar)
        return mat * 1 / scalar;
    return Matrix<T, row, col>::Zero;
}

template<class T, size_t row, size_t col>
inline Matrix<T, row, col>& operator/=(Matrix<T, row, col>& mat, T scalar)
{
    return mat = mat / scalar;
}


// matrix matrix sum/subtract


template<class T, size_t row, size_t col>
inline Matrix<T, row, col> operator+(Matrix<T, row, col> const& lhs, Matrix<T, row, col> const& rhs)
{
    Matrix<T, row, col> result;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            result.arr[i][j] = lhs.arr[i][j] + rhs.arr[i][j];
    return result;
}

template<class T, size_t row, size_t col>
inline Matrix<T, row, col> operator-(Matrix<T, row, col> const& rhs)
{
    Matrix<T, row, col> result;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            result.arr[i][j] = -rhs.arr[i][j];
    return result;
}

template<class T, size_t row, size_t col>
inline Matrix<T, row, col> operator-(Matrix<T, row, col> const& lhs, Matrix<T, row, col> const& rhs)
{
    return lhs + (-rhs);
}

template<class T, size_t row, size_t col>
inline Matrix<T, row, col>& operator+=(Matrix<T, row, col>& lhs, Matrix<T, row, col> const& rhs)
{
    return lhs = (lhs + rhs);
}

template<class T, size_t row, size_t col>
inline Matrix<T, row, col>& operator-=(Matrix<T, row, col>& lhs, Matrix<T, row, col> const& rhs)
{
    return lhs = (lhs - rhs);
}


template<class T, size_t row, size_t col>
inline bool operator==(Matrix<T, row, col> const& lhs, Matrix<T, row, col> const& rhs)
{
    bool result = true;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            result &= lhs[i][j] == rhs[i][j];
    return result;
}

template<class T, size_t row, size_t col>
inline bool operator!=(Matrix<T, row, col> const& lhs, Matrix<T, row, col> const& rhs)
{
    return !(lhs == rhs);
}