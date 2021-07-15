#ifndef LOADER_H
#define LOADER_H

#include "mesh.h"

#include <QVector>

class Loader
{
public:
    Loader();
    static void load(QVector<Mesh>& meshList);

};

#endif // LOADER_H
