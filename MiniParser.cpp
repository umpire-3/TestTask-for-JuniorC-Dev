//
// Created by umpire on 10.03.17.
//

#include "MiniParser.h"
#include "XmlException.h"

MiniParser::MiniParser():
    state(Start),
    currentTag(""),
    currentValue(""),
    handleTagStart([](std::string, long) {}),
    handleTagValue([](std::string, std::string, std::stack<std::string>) {})
{}

bool verifyTag(std::string tag) {
    if (tag.size() == 0) return false;

    char first = tag[0];
    if (!isalpha(first) && first != '_') {
        return false;
    }

    tag.erase(tag.begin());
    for (char &i : tag) {
        if(!isalnum(i) && i != '_') {
            return false;
        }
    }

    return true;
}

void MiniParser::parse(const char *filename) {
    file.open(filename);

    file.seekg(0, std::ios::end);
    buffer.reserve(file.tellg());
    file.seekg(0);

    char currentChar;
    while (file.get(currentChar)) {
        switch (state) {
            case Start:
                start(currentChar);
                break;
            case OpenTag:
                openTag(currentChar);
                break;
            case CloseTag:
                closeTag(currentChar);
                break;
            case Value:
                value(currentChar);
                break;
        }
        buffer.push_back(currentChar);
    }

    file.close();
}

void MiniParser::start(char currentChar) {
    if (currentChar == '<') {
        state = OpenTag;
        return;
    }
}

void MiniParser::openTag(char currentChar) {
    if (currentChar == '/') {
        state = CloseTag;
        return;
    }
    if (currentChar == '>') {
        if (verifyTag(currentTag)) {
            tags.push(currentTag);
            handleTagStart(currentTag, file.tellg());
            currentValue = "";
            state = Value;
        }
        else {
            throw XmlException("Invalid tag name at " + std::to_string(file.tellg()) + " char in file");
        }
        return;
    }
    currentTag += currentChar;
}

void MiniParser::value(char currentChar) {
    if (currentChar == '<') {
        currentTag = "";
        state = OpenTag;
        return;
    }
    currentValue += currentChar;
}

void MiniParser::closeTag(char currentChar) {
    if (currentChar == '>') {
        if (currentTag != tags.top()) {
            throw XmlException("Closing tag name doesn't match the previous opening tag named - " + tags.top());
        }
        else {
            tags.pop();
            handleTagValue(currentTag, currentValue, tags);
            currentValue = "";
            state = Value;
        }
        return;
    }
    currentTag += currentChar;
}

void MiniParser::onTagStart(std::function<void(std::string, long)> callback) {
    handleTagStart = callback;
}

void MiniParser::onTagValue(std::function<void(std::string, std::string, std::stack<std::string>)> callback) {
    handleTagValue = callback;
}

const std::vector<char> &MiniParser::getBuffer() const {
    return buffer;
}
