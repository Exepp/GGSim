#include <GGSim/Shape.h>
#include <GGSim/Window.h>
#include <cmath>

void Shape::draw(WindowModule& win, Transform model) const
{
    win.draw(verts, model *= getTransform());
}

Box::Box(Vec3_t const& size)
    : size(size)
{
    float x = 0.5f * size.x;
    float y = 0.5f * size.y;
    float z = 0.5f * size.z;

    verts.set({ Vertex({ -x, -y, -z }, { 1.f, 0.f, 0.f, 1.f }),
                Vertex({ -x, -y, z }, { 1.f, 0.f, 0.f, 1.f }),
                Vertex({ x, -y, -z }, { 1.f, 0.f, 0.f, 1.f }),
                Vertex({ x, -y, z }, { 1.f, 0.f, 0.f, 1.f }),
                Vertex({ -x, y, -z }, { 0.f, 0.5f, 1.f, 1.f }),
                Vertex({ -x, y, z }, { 1.f, 0.5f, 1.f, 1.f }),
                Vertex({ x, y, -z }, { 1.f, 0.f, 1.f, 1.f }),
                Vertex({ x, y, z }, { 1.f, 1.f, 1.f, 1.f }) });
    verts.set({ 0, 1, 2,
                1, 2, 3,
                0, 1, 5,
                0, 4, 5,
                1, 3, 7,
                1, 5, 7,
                2, 3, 7,
                2, 6, 7,
                0, 2, 6,
                0, 4, 6,
                4, 5, 6,
                5, 6, 7 });
}

Sphere::Sphere(float radius)
    : radius(radius)
{
    constexpr size_t CirVCnt = 30;
    constexpr size_t VCount  = (CirVCnt / 2 - 1) * CirVCnt + 2;
    const float      diff    = 2 * M_PI / CirVCnt;


    VertexArray::VertArr_t vertArr;
    vertArr.reserve(CirVCnt / 2 * CirVCnt);

    vertArr.push_back(Vertex({ 0, 1 * radius, 0 }, { 1, 0, 0, 1 }));
    for (int i = 1; i < CirVCnt / 2; ++i)
    {
        float currR = fabs(sin(i * diff)) * radius;
        float y     = cos(i * diff) * radius;
        for (int j = 0; j < CirVCnt; ++j)
            vertArr.push_back(Vertex({ sin(j * diff) * currR, y, cos(j * diff) * currR },
                                     { 1, 0, 0, 1 }));
    }
    vertArr.push_back(Vertex({ 0, -1 * radius, 0 }, { 1, 0, 0, 1 }));

    verts.set(std::move(vertArr));


    VertexArray::IdxArr_t idxArr;
    idxArr.reserve(CirVCnt / 2 * CirVCnt);

    for (int i = 0; i < CirVCnt; ++i)
    {
        idxArr.push_back(0);
        idxArr.push_back(i % CirVCnt + 1);
        idxArr.push_back((i + 1) % CirVCnt + 1);
    }

    for (int i = 1; i < CirVCnt / 2 - 1; ++i)
        for (int j = 0; j < CirVCnt; ++j)
        {
            idxArr.push_back((i - 1) * CirVCnt + j + 1);
            idxArr.push_back(i * CirVCnt + j % CirVCnt + 1);
            idxArr.push_back(i * CirVCnt + (j + 1) % CirVCnt + 1);
            idxArr.push_back((i - 1) * CirVCnt + j + 1);
            idxArr.push_back((i - 1) * CirVCnt + (j + 1) % CirVCnt + 1);
            idxArr.push_back(i * CirVCnt + (j + 1) % CirVCnt + 1);
        }

    int lastCIdx = VCount - CirVCnt - 1 - 1;
    for (int i = 0; i < CirVCnt; ++i)
    {
        idxArr.push_back(lastCIdx + i % CirVCnt + 1);
        idxArr.push_back(lastCIdx + (i + 1) % CirVCnt + 1);
        idxArr.push_back(VCount - 1);
    }

    verts.set(std::move(idxArr));
}