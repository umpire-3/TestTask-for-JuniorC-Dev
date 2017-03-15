//
// Created by umpire on 11.03.17.
//

#ifndef JUNTESTTASK_XMLWRITER_H
#define JUNTESTTASK_XMLWRITER_H


#include <mutex>
#include <vector>

class XmlWriter {
    friend class XmlWriter_write_Test;
    friend class XmlWriter_save_Test;

    std::mutex mutex;
    std::vector<char> buffer;
    std::vector<char>::iterator pos;
    long posPointer = 0;

public:

    void setOutputPos(long);
    void setBuffer(const std::vector<char>&);
    void write(int);
    void save(const char*);
};


#endif //JUNTESTTASK_XMLWRITER_H
