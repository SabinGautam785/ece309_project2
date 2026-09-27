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
#include "harness/harness.h"
#include "model/replay_client.h"
#include "model/scripted_client.h"
#include <vector>
#include <cassert>
#include <utility>
#include <stdexcept>

class TestInput : public InputSource {
    public:

    TestInput(std::vector<std::string> lines) : lines_(lines) {
        
    }
    std::string read_line() override {
        if (index_ < lines_.size()) {
            std::string result = lines_[index_];
            index_++;
            return result;
        }
        eof_ = true;
        return "";
    }
    bool is_eof() const override {
        return eof_;
    }
    private:
    std::vector<std::string> lines_;
    std::size_t index_ = 0;
    bool eof_ = false;
};
class TestOutput : public OutputSink {
    public:
    
    std::string text;
    void write(std::string_view chunk) override {
        text += chunk;
    }
};
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
    assert(copy.begin() != conversation.begin());
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
    const Message* originalPtr = moveSource.begin();
    Conversation moved(std::move(moveSource));
    assert(moved.begin() == originalPtr);
    assert(moved.size() == 1);
    assert(moved.at(0).content() == "Move Test");
    assert(moveSource.size() == 0);
    assert(moveSource.begin() == nullptr);
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
    //conversation growth test
    Conversation growthTest;
    Message growthMessage1(Role::User, "One");
    growthTest.append(growthMessage1);
    const Message* growthPtr1 = growthTest.begin();
    Message growthMessage2(Role::User, "Two");
    growthTest.append(growthMessage2);
    const Message* growthPtr2 = growthTest.begin();
    assert(growthPtr1 != growthPtr2);
    Message growthMessage3(Role::User, "Three");
    growthTest.append(growthMessage3);
    const Message* growthPtr3 = growthTest.begin();
    assert(growthPtr2 != growthPtr3);
    Message growthMessage4(Role::User, "Four");
    growthTest.append(growthMessage4);
    const Message* growthPtr4 = growthTest.begin();
    assert(growthPtr3 == growthPtr4);
    //Conv Order Test
    Conversation orderTest;
    Message conversationMsg1(Role::System, "System inst");
    orderTest.append(conversationMsg1);
    Message conversationMsg2(Role::User, "Heyy");
    orderTest.append(conversationMsg2);
    assert(orderTest.at(0).role() == Role::System);
    assert(orderTest.at(1).role() == Role::User);
    //Empty bound test
    Conversation emptyBounds;
    assert(emptyBounds.size() == 0);
    assert(emptyBounds.begin() == nullptr);
    assert(emptyBounds.begin() == emptyBounds.end());
    bool caughtOutOfRange = false;
    try {
        emptyBounds.at(0);
    }
    catch (const std::out_of_range&) {
        caughtOutOfRange = true;
    }
    assert(caughtOutOfRange == true);
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
    std::string sentinel = "<|end_conversation|>";
    std::string splitText = "Goodbye.<|end_conversation|>";
    for (std::size_t i = 0; i < splitText.size(); i++) {
      std::string first = splitText.substr(0, i);
      std::string second = splitText.substr(i);
       SentinelScanner scanner(sentinel);
       SentinelScanner::Out part1 = scanner.feed(first);
       assert(part1.sentinel_found == false);
       SentinelScanner::Out part2 = scanner.feed(second);
       assert(part2.sentinel_found == true);
       assert(part1.safe_text + part2.safe_text == "Goodbye.");
    }
    
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
    //Scannerbound test
    SentinelScanner boundedScanner("<|end_conversation|>");
    std::size_t pendingLimit = sentinel.size() - 1;
    for (std::size_t i = 0; i < 10000; i++) {
        SentinelScanner::Out bout = boundedScanner.feed("<");
        assert(bout.sentinel_found == false);
        if (i >= pendingLimit) {
            assert(bout.safe_text.size() == 1);
        }
    }
    //harness test
    {
        auto model = std::make_unique<ScriptedModelClient>("scripts/greeting.script");
        HarnessConfig cfg;
        cfg.max_turns = 2;
        Harness harness(std::move(model), cfg);
        TestInput input({"Hello", "Thanks"});
        TestOutput output;
        StopReason reason = harness.run(input, output);
        assert(reason.kind == StopReason::Kind::TurnLimit);
    }
    {
        auto model = std::make_unique<ScriptedModelClient>("scripts/greeting.script");
        HarnessConfig cfg;
        cfg.max_turns = 3;
        Harness harness(std::move(model), cfg);
        TestInput input({"Hello", "Thanks", "Bye"});
        TestOutput output;
        StopReason reason = harness.run(input, output);
        assert(reason.kind == StopReason::Kind::Sentinel);
    }
    //Transcript Round trip test
    ReplayModelClient replay("transcript.txt");
    Conversation replayConversation;
    Message replayMessage1 = replay.generate(replayConversation);
    assert(replayMessage1.role() == Role::Assistant);
    assert(replayMessage1.content() == "I am doing well, thank you! How can I help you?");
    Message replayMessage2 = replay.generate(replayConversation);
    assert(replayMessage2.role() == Role::Assistant);
    assert(replayMessage2.content() == "I can definitely do that for you. Anything else?");
    Message replayMessage3 = replay.generate(replayConversation);
    assert(replayMessage3.role() == Role::Assistant);
    assert(replayMessage3.content() == "Goodbye!<|end_conversation|>");
    assert(replay.system_message() == "Be concise.");
    return 0;
}

