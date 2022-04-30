#pragma once

#ifndef CHUNK_H
#define CHUNK_H
#include <stdlib.h>  

#define CHUNK_VOLUME 65536
#define CHUNK_WIDTH 16
class Chunk {

    public:
        char map[CHUNK_VOLUME];

        Chunk(){
            for (int i = 0; i < CHUNK_VOLUME; ++i) {
                map[i] = 'a';
            }
            for (int i = 0; i < CHUNK_WIDTH * CHUNK_WIDTH * 10; ++i) {
                map[i] = 'b';
            }
        }

};

#endif