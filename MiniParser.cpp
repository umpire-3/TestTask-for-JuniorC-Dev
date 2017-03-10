//
// Created by umpire on 10.03.17.
//

#include "MiniParser.h"

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

void MiniParser::parse(const char *file) {
    in.open(file);
    in.seekg(0, std::ios::end);
    buffer.reserve(in.tellg());
    in.seekg(0);
    while (!in.eof()) {
        char currentChar;
        in.read(&currentChar, 1);
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
    in.close();
}

void MiniParser::start(char currentChar) {
    if (currentChar == '<') {
        state = OpenTag;
        return;
    }
    else {
        //throw xml exception here
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
            handleTagStart(currentTag, in.tellg());
            currentValue = "";
            state = Value;
        }
        else {
            //throw xml exception here
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
            //throw xml exception here
        }
        else {
            handleTagValue(currentTag, currentValue, tags);
            currentValue = "";
            state = Value;
            tags.pop();
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
