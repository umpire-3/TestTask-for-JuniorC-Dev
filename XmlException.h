//
// Created by umpire on 15.03.17.
//

#ifndef JUNTESTTASK_XMLEXCEPTION_H
#define JUNTESTTASK_XMLEXCEPTION_H


#include <string>

class XmlException {
    std::string message;
public:
    XmlException(std::string);

    const std::string &getMessage() const;
};


#endif //JUNTESTTASK_XMLEXCEPTION_H
