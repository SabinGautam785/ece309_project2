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
//#include "core/sentinel_scanner.h"
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
    return 0;
}
