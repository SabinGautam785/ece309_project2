# Design Log — Project 2

(500–800 words total. See spec §5 for what each section must cover.)

## Growth factor and amortized cost
I used the geometrical growth strategy with factor of 2. My Conversation begins with capacity_= 0. When the first messgae was appended I created space for 1 message. During the growth, I allocated a new message array with larger capacity. I copied the existimng messgaes into it, deleted the old array and pointed data_ to the new array. Doubling really helped me as I did not have to allocate the new array everytime the message was appended. However, the growth operation must be expensive as I have to copy all existing messages. In spite of the cost resizing does not happen on every append. As the average cost per append remain consatnt, geometric growth gives amortized O(1) behavior. My growth test recorded the begin() pointer and it only changed when capacity had to grow.



## Rule of Five evidence
Conversation owns dynamically allocate dmemory through Message* data_. Therefore copying the pointers would cause two objects to share same allocation and cause memory-management problems. Therfore I implemented rule of five in conversation.cpp and tested it in test_p2.cpp. My destructor used delete[] data_ to release the dynamically allocated array. The copy constructor created new array and copied each existing Message giving the copy its own storage. The copy asignment handeled self-assignment, released the old array, allocated storage based on other conversation and copied its messages. Move constructor transfeered data_, size_, and caapcity_ from source and it reset the source to nullptr, 0 and 0. Move assignment used similar idea but released destination existing memory before taking the ownership. Copy test check the contents weer equal while: 
copy.begin() != conveersation.begin(). Move test saved the original pointer an dverified that the moved object also has the same pointer.


## Sentinel scanner: bounded pending_ proof
My sentinel is <|end_conversation|>. As the scanner may recieve sentinel split across multiple chunks it cannot output every character it recieves. Temorarily characters are stored in pending_. The code acalculates maximum amount to keep using : sentinel_.size() - 1. If combined text is larger than the limit the extra characters are returnes as safe_text. When the complete santinel is not found pending_ is kept bounded by sentinel minus one. Its memory usage does not grow with total amount of streamed text. However, if complete sentinel is detected sentinel_found = true is reported and it clears the pending_. The tests were fed data repeatedly including one character at a time and 10000 ilteration bounded-scanner test to verify continue release of safe text.



## What I would change differently
I would plan the required test earlier ratther than implemeenting first and strengthening those tests later. I would organize my tests into smaller blocks rather than keeping them under the larger main function. I would test the sentinel scanner at chunk boundries earlier as it was the most trickiest part. I will also try to make frequent Git comments. However I would keep the simple doubling strategy since it mad ethe things really easy to understand and provided amortized constant time appends.
