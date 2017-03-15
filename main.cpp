#include <thread>
#include <vector>
#include <iostream>
#include "interval.h"
#include "MiniParser.h"
#include "XmlWriter.h"
#include "XmlException.h"

XmlWriter xmlWriter;

bool isPrime(int n) {
    for (int i = 2; i <= n/2; i++) {
        if (n%i == 0) return false;
    }
    return true;
}

void genPrimes(Interval interval) {
    for(int i = interval.low; i < interval.high; i++) {
        if (isPrime(i)) {
            xmlWriter.write(i);
        }
    }
}

int main(int argc, char **argv) {
    MiniParser xmlParser;

    xmlParser.onTagStart([] (std::string tagName, long pos) {
        if (tagName == "primes") {
            xmlWriter.setOutputPos(pos);
        }
    });

    int low;
    std::vector<Interval> intervals;
    xmlParser.onTagValue([&] (std::string tagName, std::string tagValue, std::stack<std::string> tags) {
        if (!tags.empty() && tags.top() == "interval") {
            tags.pop();
            if (!tags.empty() && tags.top() == "intervals") {
                tags.pop();
                if (!tags.empty() && tags.top() == "root") {
                    if (tagName == "low") {
                        low = std::stoi(tagValue);
                    }
                    else if (tagName == "high") {
                        int high = std::stoi(tagValue);
                        intervals.push_back(Interval{low, high});
                    }
                }
            }
        }
    });

    try {
        xmlParser.parse("data.xml");

        xmlWriter.setBuffer(xmlParser.getBuffer());

        std::vector<std::thread> threads;
        for (auto &interval : intervals) {
            threads.push_back(std::thread(genPrimes, interval));
        }

        for (auto &thread : threads) {
            thread.join();
        }

        xmlWriter.save("data.xml");

    }
    catch (XmlException e) {
        std::cout << e.getMessage() << "\n";
    }

    return 0;
}
