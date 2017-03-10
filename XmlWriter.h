//
// Created by umpire on 11.03.17.
//

#ifndef JUNTESTTASK_XMLWRITER_H
#define JUNTESTTASK_XMLWRITER_H


#include <mutex>
#include <vector>

class XmlWriter {
    std::mutex mutex;
    std::vector<char> buffer;
    std::vector<char>::iterator pos;
    long posPointer;

public:

    void setOutputPos(long);
    void setBuffer(const std::vector<char>&);
    void write(int);
    void save(const char*);
};


#endif //JUNTESTTASK_XMLWRITER_H
