#include <GGSim/Application.h>
#include <GGSim/VertexArray.h>

Vertex operator*(Mat4_t const& mat, Vertex const& v)
{
    return { mat * Vec4_t(v.pos, 1.f), v.clr };
}

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vertex::pos));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

VertexArray::VertexArray(VertexArray const& obj)
    : VertexArray(obj.verts, obj.idxs)
{
}

VertexArray::VertexArray(VertexArray&& obj)
{
    *this = std::move(obj);
}

VertexArray& VertexArray::operator=(VertexArray const& obj)
{
    return *this = VertexArray(obj);
}

VertexArray& VertexArray::operator=(VertexArray&& obj)
{
    vao   = obj.vao;
    vbo   = obj.vbo;
    ebo   = obj.ebo;
    verts = std::move(obj.verts);
    idxs  = std::move(obj.idxs);

    obj.vao = 0;
    obj.vbo = 0;
    obj.ebo = 0;

    return *this;
}

VertexArray::VertexArray(VertArr_t vertsArr, IdxArr_t idxArr)
    : VertexArray()
{
    set(std::move(vertsArr));
    set(std::move(idxArr));
}

VertexArray::VertexArray(std::initializer_list<Vertex> vertsArr, std::initializer_list<Idx_t> idxArr)
    : VertexArray()
{
    set(vertsArr);
    set(idxArr);
}

void VertexArray::set(std::initializer_list<Vertex> vertsArr)
{
    verts = VertArr_t(vertsArr);
    updateVerts();
}

void VertexArray::set(std::initializer_list<Idx_t> idxArr)
{
    idxs = IdxArr_t(idxArr);
    updateIdxs();
}

void VertexArray::set(VertArr_t vertsArr)
{
    verts = std::move(vertsArr);
    updateVerts();
}

void VertexArray::set(IdxArr_t idxArr)
{
    idxs = std::move(idxArr);
    updateIdxs();
}

void VertexArray::updateVerts() const
{
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(verts[0]), verts.data(), GL_STATIC_DRAW);
}

void VertexArray::updateIdxs() const
{
    glBindVertexArray(vao);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxs.size() * sizeof(idxs[0]), idxs.data(), GL_STATIC_DRAW);
}

VertexArray::~VertexArray()
{
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}


void VertexArray::draw() const
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, idxs.size(), GL_UNSIGNED_INT, 0);
}