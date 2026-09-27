// tests/p2/test_p2.cpp
//
// YOUR test suite goes here. At least 12 assert-based test cases — see
// spec §5 for the required categories and the sample test for the
// expected level of rigor.
//
// This file is a stub so the project builds out of the box; replace the
// body of main() with your own tests.

#include "core/conversation.h"
#include "core/message.h"
#include "core/sentinel_scanner.h"
//#include "harness/harness.h"
//#include "model/replay_client.h"
//#include "model/scripted_client.h"

#include <cassert>
#include <utility>

int main() {
    Conversation conversation;
    assert(conversation.size() == 0);
    Message message(Role::User, "Hello");
    conversation.append(message);
    assert(conversation.size() == 1);
    conversation.at(0).content();
    assert(conversation.at(0).content() == "Hello");
    Message message2(Role::User, "Second");
    conversation.append(message2);
    assert(conversation.size() == 2);
    assert(conversation.at(1).content() == "Second");
    //Copy const test
    Conversation copy(conversation);
    assert(copy.size() == 2);
    assert(copy.at(0).content() == "Hello");
    assert(copy.at(1).content() == "Second");
    //copy asisgn const test
    Conversation assigned;
    assigned = conversation;
    assert(assigned.size() == 2);
    assert(assigned.at(0).content() == "Hello");
    assert(assigned.at(1).content() == "Second");
    //move const test
    Conversation moveSource;
    Message moveMessage(Role::User, "Move Test");
    moveSource.append(moveMessage);
    Conversation moved(std::move(moveSource));
    assert(moved.size() == 1);
    assert(moved.at(0).content() == "Move Test");
    //testing whether moved from object
    assert(moveSource.size() == 0);
    //testing move assignment
    Conversation moveAssigned;
    Conversation moveAssignedSource;
    Message moveAssignMessage(Role::User, "Move assign text");
    moveAssignedSource.append(moveAssignMessage);
    moveAssigned = std::move(moveAssignedSource);
    assert(moveAssigned.size() == 1);
    assert(moveAssigned.at(0).content() == "Move assign text");
    assert(moveAssignedSource.size() == 0);
    //begin() test
    const Message* first = conversation.begin();
    assert(first->content() == "Hello");
    //end() test
    const Message* last = conversation.end();
    assert((last - first) == 2);
    //Sentinel Scanner test
    SentinelScanner scanner("<|end_conversation|>");
    SentinelScanner::Out out = scanner.feed("Hello World");
    assert(out.sentinel_found == false);
    SentinelScanner::Out flushed = scanner.flush();
    assert(out.safe_text + flushed.safe_text == "Hello World");
    SentinelScanner scanner2("<|end_conversation|>");
    SentinelScanner::Out out2 = scanner2.feed("GoodBye<|end_conversation|>");
    assert(out2.safe_text == "GoodBye");
    assert(out2.sentinel_found == true);
    SentinelScanner scanner3("<|end_conversation|>");
    SentinelScanner::Out part1 = scanner3.feed("GoodBye<|end_");
    assert(part1.sentinel_found == false);
    SentinelScanner::Out part2 = scanner3.feed("conversation|>");
    assert(part2.sentinel_found == true);
    assert(part1.safe_text + part2.safe_text == "GoodBye");
    SentinelScanner scanner4("<|end_conversation|>");
    SentinelScanner::Out falseMatch = scanner4.feed("Hello <|end_NOT_A_SENTINEL");
    assert(falseMatch.sentinel_found == false);
    SentinelScanner::Out falseflushed = scanner4.flush();
    assert(falseMatch.safe_text + falseflushed.safe_text == "Hello <|end_NOT_A_SENTINEL");
    SentinelScanner scanner5("<|end_conversation|>");
    std::string sentinelText = "<|end_conversation|>";
    bool found = false;
    for (char c : sentinelText) {
        std::string oneChar(1, c);
        SentinelScanner::Out charResult = scanner5.feed(oneChar);
        if (charResult.sentinel_found) {
            found = true;
        }
    }
    assert(found == true);
    SentinelScanner scanner6("<|end_conversation|>");
    std::string longText(100, 'A');
    SentinelScanner::Out longResult = scanner6.feed(longText);
    std::size_t maxPending = sentinelText.size() - 1;
    assert(longResult.safe_text.size() >= longText.size() - maxPending);
    return 0;
}

