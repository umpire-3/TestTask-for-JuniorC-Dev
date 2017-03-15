//
// Created by umpire on 15.03.17.
//

#include "XmlException.h"

XmlException::XmlException(std::string msg):
    message(msg)
{}

const std::string &XmlException::getMessage() const {
    return message;
}
