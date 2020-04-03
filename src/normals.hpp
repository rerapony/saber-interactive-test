#pragma once

#include <glm/glm.hpp>
using namespace glm;

// задача делалать на Убунту, поэтому код не компилировался (кажестя, с glm лучше работать под Виндой),
// но вроде бы он должен работать без проблем
void calc_mesh_normals(vector* normals, const vec3* verts, const int* faces, int nverts, int nfaces)
{
    // инициализируем нормали нулями
    for (int i = 0; i < nverts; ++i) {
        normals[i] = vec3(0.0, 0,0, 0.0);
    }

    int a, b, c;
    vec3 n;

    //кажется, вершины идут друг за другом в массиве мешей?
    for (int i = 0; i < nfaces; ++i) {
        a = faces[3*i];
        b = faces[3*i+1];
        c = faces[3*i+2];

        n = cross(verts[b] - verts[a], verts[c] - verts[a]);

        normals[a] += n;
        normals[b] += n;
        normals[c] += n;
    }

    //мы можем не считать среднее значение в цикле выше, т.к. все равно нормализуем вектор в конце
    for (int i = 0; i < nverts; ++i) {
        normals[i] = normalize(normals[i]);
    }
}