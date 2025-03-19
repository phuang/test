#!/usr/bin/env python3
# -*- coding: utf-8 -*-

cubeVertexArray = [
    # float4 position, float4 color, float2 uv,
    1, -1, 1, 1, 1, 0, 1, 1, 0, 1,
    -1, -1, 1, 1, 0, 0, 1, 1, 1, 1,
    -1, -1, -1, 1, 0, 0, 0, 1, 1, 0,
    1, -1, -1, 1, 1, 0, 0, 1, 0, 0,
    1, -1, 1, 1, 1, 0, 1, 1, 0, 1,
    -1, -1, -1, 1, 0, 0, 0, 1, 1, 0,

    1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, -1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, -1, -1, 1, 1, 0, 0, 1, 1, 0,
    1, 1, -1, 1, 1, 1, 0, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, -1, -1, 1, 1, 0, 0, 1, 1, 0,

    -1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, -1, 1, 1, 1, 0, 1, 1, 0,
    -1, 1, -1, 1, 0, 1, 0, 1, 0, 0,
    -1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
    1, 1, -1, 1, 1, 1, 0, 1, 1, 0,

    -1, -1, 1, 1, 0, 0, 1, 1, 0, 1,
    -1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
    -1, 1, -1, 1, 0, 1, 0, 1, 1, 0,
    -1, -1, -1, 1, 0, 0, 0, 1, 0, 0,
    -1, -1, 1, 1, 0, 0, 1, 1, 0, 1,
    -1, 1, -1, 1, 0, 1, 0, 1, 1, 0,

    1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    -1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
    -1, -1, 1, 1, 0, 0, 1, 1, 1, 0,
    -1, -1, 1, 1, 0, 0, 1, 1, 1, 0,
    1, -1, 1, 1, 1, 0, 1, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 1,

    1, -1, -1, 1, 1, 0, 0, 1, 0, 1,
    -1, -1, -1, 1, 0, 0, 0, 1, 1, 1,
    -1, 1, -1, 1, 0, 1, 0, 1, 1, 0,
    1, 1, -1, 1, 1, 1, 0, 1, 0, 0,
    1, -1, -1, 1, 1, 0, 0, 1, 0, 1,
    -1, 1, -1, 1, 0, 1, 0, 1, 1, 0,
]

stride = 10

def indexiz(vertices, indexRange):
    index = 0
    indexMap = {}
    indices = []
    vertices = [ tuple(vertices[i:i + stride]) for i in range(0, len(vertices), stride) ]

    count = 0

    outputVertices = []

    for i, key in enumerate(vertices):
        if count == indexRange:
            count = 0
            indexMap.clear()
        count += 1
        if key in indexMap:
            indices.append(indexMap[key])
        else:
            indexMap[key] = index
            indices.append(index)
            outputVertices.append(list(key))
            index += 1
    return outputVertices, indices

vertices, indices = indexiz(cubeVertexArray, 36)

for v in vertices:
    print(", ".join(["%d" % i for i in v]) + ",")

for i in range(0, len(indices), 3):
    print(",".join("%d"%index for index in indices[i:i + 3]) + ",")
