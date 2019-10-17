#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <glad/glad.h>
#include <vector>


struct Vertex
{
    Vertex(Vec3_t const& pos = Vec3_t(0, 0, 0), Vec4_t const& clr = Vec4_t(0, 0, 0, 1))
        : pos(pos)
        , clr(clr)
    {}

    Vec3_t pos;

    Vec4_t clr;
};

Vertex operator*(Mat4_t const& mat, Vertex const& v);


class VertexArray
{
public:
    using Vert_t = Vertex;

    using VertArr_t = std::vector<Vert_t>;

    using Idx_t = GLuint;

    using IdxArr_t = std::vector<Idx_t>;

public:
    VertexArray();

    VertexArray(VertexArray const&) = delete;

    VertexArray(VertexArray&& obj);

    VertexArray& operator=(VertexArray const&) = delete;

    VertexArray& operator=(VertexArray&& obj);

    VertexArray(VertArr_t vertsArr, IdxArr_t idxArr);

    VertexArray(std::initializer_list<Vertex> vertsArr, std::initializer_list<Idx_t> idxArr);

    ~VertexArray();


    void set(std::initializer_list<Vertex> vertsArr);

    void set(std::initializer_list<Idx_t> idxArr);

    void set(VertArr_t vertsArr);

    void set(IdxArr_t idxArr);


    void draw() const;


    VertArr_t const& getVertsArr() const;

    IdxArr_t const& getIdxsArr() const;

private:
    void updateVerts() const;

    void updateIdxs() const;


private:
    VertArr_t verts;

    IdxArr_t idxs;

    GLuint vao, vbo, ebo;
};

#endif // VERTEXARRAY_H