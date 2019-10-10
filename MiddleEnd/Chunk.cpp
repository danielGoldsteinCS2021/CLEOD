//
// Created by jonwi on 10/9/2019.
//

#include "Chunk.h"

#include <iostream>
//#include <string>

const uint64_t BYTE_MASK = 0xFF;

clint readInt(const std::vector<byte> &bytes, uint pos) {
    clint val = 0;
    for(uint j = 0; j < 8; j++)
        val = val | (static_cast<clint>(bytes[pos + j]) << 8 * (j));
    return val;
}
void writeInt(std::vector<byte> &bytes, clint val) {
    for(int j = 0; j < 8; j++) {
        bytes.push_back((val & (BYTE_MASK << 8 * j)) >> 8 * j);
    }
}
cluint readUint(const std::vector<byte> &bytes, uint pos) {
    return static_cast<cluint>(readInt(bytes, pos));
}
void writeUint(std::vector<byte> &bytes, cluint val) {
    for(int j = 0; j < 8; j++)
        bytes.push_back((val & (BYTE_MASK << 8 * j)) >> 8 * j);
}