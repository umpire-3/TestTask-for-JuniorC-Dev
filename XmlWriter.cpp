//
// Created by umpire on 11.03.17.
//

#include "XmlWriter.h"


void XmlWriter::setOutputPos(long pos) {
    posPointer = pos;
}

void XmlWriter::setBuffer(const std::vector<char> &buffer) {
    this->buffer = buffer;
    pos = this->buffer.begin() + posPointer;
}

void XmlWriter::write(int val) {
    std::lock_guard<std::mutex> lock(mutex);
    std::string s = std::to_string(val) + " ";
    pos = buffer.insert(pos, s.begin(), s.end()) + s.size();
}

void XmlWriter::save(const char *file) {
    std::ofstream out(file);
    out.write(buffer.begin().base(), buffer.size() - 1);
}
