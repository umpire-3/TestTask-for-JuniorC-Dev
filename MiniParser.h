//
// Created by umpire on 10.03.17.
//

#ifndef JUNTESTTASK_MINIPARSER_H
#define JUNTESTTASK_MINIPARSER_H


#include <fstream>
#include <stack>
#include <vector>
#include <functional>

class MiniParser {
    enum State {
        Start,
        OpenTag,
        Value,
        CloseTag
    };
    State state;
    std::string currentTag, currentValue;
    std::stack<std::string> tags;

    std::ifstream in;
    std::vector<char> buffer;

    std::function<void(std::string, long)> handleTagStart;
    std::function<void(std::string, std::string, std::stack<std::string>)> handleTagValue;

    void start(char);
    void openTag(char);
    void closeTag(char);
    void value(char);

public:
    MiniParser();

    void parse(const char*);

    void onTagStart(std::function<void(std::string, long)>);
    void onTagValue(std::function<void(std::string, std::string, std::stack<std::string>)>);

    const std::vector<char> &getBuffer() const;
};


#endif //JUNTESTTASK_MINIPARSER_H
