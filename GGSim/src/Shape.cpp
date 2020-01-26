#include <GGSim/Shape.h>
#include <GGSim/Window.h>
#include <cmath>


void Shape::draw(WindowModule& win, Transform model) const
{
    win.draw(verts, model);
}


Shape::VertArr_t& Shape::getVertexArray()
{
    return verts;
}

Shape::VertArr_t const& Shape::getVertexArray() const
{
    return verts;
}


//
//////////////////////////////// Box ////////////////////////////////
//


Box::Box(Vec3_t const& size)
    : cShape(toCVec3(size * 0.5f))
    , size(size)
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
    verts.set({ 0, 2, 1, 1, 2, 3,
                0, 1, 5, 0, 5, 4,
                1, 3, 7, 1, 7, 5,
                2, 7, 3, 2, 6, 7,
                0, 6, 2, 0, 4, 6,
                4, 5, 6, 5, 7, 6 });
}

Mat3_t Box::computeInertiaTensorNoMass() const
{
    static constexpr float const one_12 = 1.f / 12.f;

    Mat3_t tensor(0);
    float  ww = size.x * size.x;
    float  hh = size.y * size.y;
    float  dd = size.z * size.z;

    tensor[0][0] = one_12 * (hh + dd);
    tensor[1][1] = one_12 * (ww + dd);
    tensor[2][2] = one_12 * (ww + hh);

    return tensor;
}


//
//////////////////////////////// Sphere ////////////////////////////////
//


Sphere::Sphere(float radius)
    : radius(radius)
    , cShape(radius)
{
    constexpr uint_t CirVCnt = 30; // single circle vertex count
    constexpr uint_t Height  = CirVCnt / 2;
    constexpr uint_t VCount  = Height * CirVCnt + 2; // all vertices count (CircleVertexCount * height + 2)
    constexpr float  WDiff   = 2 * M_PI / CirVCnt;
    constexpr float  HDiff   = M_PI / (Height + 1); // +1, so the last level is not at the bottom


    VertexArray::VertArr_t vertArr;
    vertArr.reserve(VCount);

    vertArr.push_back(Vertex({ 0, 1 * radius, 0 }, { 1, 0, 0, 1 }));
    for (int_t i = 1; i <= Height; ++i)
    {
        float currR = fabs(sin(i * HDiff)) * radius;
        float y     = cos(i * HDiff) * radius;
        for (int_t j = 0; j < CirVCnt; ++j)
            vertArr.push_back(Vertex({ sin(j * WDiff) * currR, y, cos(j * WDiff) * currR },
                                     { 1, 0, 0, 1 }));
    }
    vertArr.push_back(Vertex({ 0, -1 * radius, 0 }, { 1, 0, 0, 1 }));

    verts.set(std::move(vertArr));


    VertexArray::IdxArr_t idxArr;
    idxArr.reserve(Height * CirVCnt * 6);

    for (int_t i = 0; i < CirVCnt; ++i)
    {
        idxArr.push_back(1 + i % CirVCnt);
        idxArr.push_back(1 + (i + 1) % CirVCnt);
        idxArr.push_back(0);
    }

    for (int_t i = 1; i < Height; ++i)
        for (int_t j = 0; j < CirVCnt; ++j)
        {
            idxArr.push_back(1 + (i - 1) * CirVCnt + j);
            idxArr.push_back(1 + i * CirVCnt + j % CirVCnt);
            idxArr.push_back(1 + i * CirVCnt + (j + 1) % CirVCnt);

            idxArr.push_back(1 + (i - 1) * CirVCnt + (j + 1) % CirVCnt);
            idxArr.push_back(1 + (i - 1) * CirVCnt + j);
            idxArr.push_back(1 + i * CirVCnt + (j + 1) % CirVCnt);
        }

    int_t lastCIdx = VCount - CirVCnt - 1 - 1;
    for (int_t i = 0; i < CirVCnt; ++i)
    {
        idxArr.push_back(VCount - 1);
        idxArr.push_back(1 + lastCIdx + (i + 1) % CirVCnt);
        idxArr.push_back(1 + lastCIdx + i % CirVCnt);
    }

    verts.set(std::move(idxArr));
}

Mat3_t Sphere::computeInertiaTensorNoMass() const
{
    static float const two_5 = 2.f / 5.f;

    Mat3_t tensor(0);
    float  rr2_5 = two_5 * radius * radius;

    tensor[0][0] = rr2_5;
    tensor[1][1] = rr2_5;
    tensor[2][2] = rr2_5;

    return tensor;
}


//
//////////////////////////////// Polyhedron ////////////////////////////////
//


// Polyhedron::Polyhedron()
//     : cvArr(0, nullptr, 0, nullptr, 0, 0, nullptr,
//             CVertArr_t::VertexDataType::VERTEX_FLOAT_TYPE,
//             CVertArr_t::IndexDataType::INDEX_INTEGER_TYPE)
//     , mesh(&cvArr)
//     , cShape(&mesh)
// {}

// void Polyhedron::updateCollisionArr()
// {
//     updateFaces();
//     auto& vertsArr = verts.getVertsArr();
//     auto& idxsArr  = verts.getIdxsArr();

//     cvArr = CVertArr_t(vertsArr.size(),
//                        vertsArr.data(), sizeof(VertArr_t::Vert_t),
//                        idxsArr.data(), sizeof(VertArr_t::Idx_t),
//                        faces.size(), faces.data(),
//                        CVertArr_t::VertexDataType::VERTEX_FLOAT_TYPE,
//                        CVertArr_t::IndexDataType::INDEX_INTEGER_TYPE);
// }

// void Polyhedron::updateFaces()
// {
//     uint idxCount = verts.getIdxsArr().size();
//     faces.clear();
//     faces.reserve(idxCount / 3);

//     for (uint i = 0; i < idxCount; i += 3)
//         faces.push_back({ 3, i });
// }

// Mat3_t Polyhedron::computeInertiaTensorNoMass() const
// {
//     rp3d::AABB aabb;
//     cShape.computeAABB(aabb, CTransform_t());

//     auto min = aabb.getMin();
//     auto max = aabb.getMax();

//     auto size = max - min;

//     Mat3_t tensor(0);
//     float  m_12 = 1.f / 12.f;
//     float  ww   = size.x * size.x;
//     float  hh   = size.y * size.y;
//     float  dd   = size.z * size.z;

//     tensor[0][0] = m_12 * (hh + dd);
//     tensor[1][1] = m_12 * (ww + dd);
//     tensor[2][2] = m_12 * (ww + hh);

//     return tensor;
// }