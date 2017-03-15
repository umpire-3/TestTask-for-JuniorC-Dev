//
// Created by umpire on 14.03.17.
//

#include <gtest/gtest.h>
#include <fstream>
#include "../XmlWriter.h"
#include "../MiniParser.h"

TEST(MiniParser, testVerifyTagFunction) {
    std::string legalTagName = "_1ce2",
        illegalTagName = "23fs4/*fsdf";
    
    ASSERT_TRUE(verifyTag(legalTagName));
    ASSERT_FALSE(verifyTag(illegalTagName));
}

TEST(MiniParser, states) {
    MiniParser parser;

    parser.onTagStart([&](std::string tagName, long pos) {
        ASSERT_EQ(parser.state, MiniParser::OpenTag);
    });

    parser.onTagValue([&](std::string tagName, std::string tagValue, std::stack<std::string> tags) {
        ASSERT_EQ(parser.state, MiniParser::CloseTag);
    });

    ASSERT_EQ(parser.state, MiniParser::Start);

    parser.parse("data.xml");
}

TEST(MiniParser, xmlStructure) {
    MiniParser parser;

    parser.onTagValue([](std::string tagName, std::string tagValue, std::stack<std::string> tags) {
        if (tagName == "low" || tagName == "high") {
            std::stack<std::string> expected_tags;
            expected_tags.push("root");
            expected_tags.push("intervals");
            expected_tags.push("interval");
            ASSERT_EQ(expected_tags.size(), tags.size());
            while (!tags.empty()) {
                ASSERT_EQ(tags.top(), expected_tags.top());
                expected_tags.pop();
                tags.pop();
            }
        }
    });

    parser.parse("data.xml");
}

TEST(XmlWriter, save) {
    XmlWriter xmlWriter;

    std::string test_string = "testing xml writer output to file",
            actual_string = "";

    xmlWriter.setBuffer(std::vector<char>(test_string.begin(), test_string.end()));
    xmlWriter.save("test_data.txt");

    {
        std::ifstream in("test_data.txt");
        char c;
        while(in.get(c)) {
            actual_string += c;
        }
    }

    ASSERT_EQ(test_string, actual_string);
}

TEST(XmlWriter, write) {
    XmlWriter xmlWriter;

    int expected_ints[3] = { 23, 5, 2356 };
    std::string expected_buffer = "<primes></primes>",
        actual_buffer = expected_buffer;
    unsigned long pos = expected_buffer.find("><") + 1;

    for (int i = 2; i >= 0; i--) {
        expected_buffer.insert(pos, std::to_string(expected_ints[i]) + " ");
    }

    xmlWriter.setOutputPos(pos);
    xmlWriter.setBuffer(std::vector<char>(actual_buffer.begin(), actual_buffer.end()));
    for (int i = 0; i < 3; i++) {
        xmlWriter.write(expected_ints[i]);
    }
    actual_buffer.assign(xmlWriter.buffer.begin(), xmlWriter.buffer.end());


    ASSERT_EQ(expected_buffer, actual_buffer);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
