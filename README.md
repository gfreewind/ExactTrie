# ExactTrie

# 概要

对于需要精准匹配大量字符串，或者要精准的匹配字符串前缀时，一般的作法是
通过hash运算，将这些字符串分散在各个桶中。查询的时候，先对数据做hash运算，
然后定位到某个桶内，再一一比较。

当有比较相近的字符串时，可能会导致一个桶内有多个字符串。
这样定位到具体的桶后，一一比较仍然比较消耗时间。

这时就需要利用Trie树来同时查询多个字符串。但传统的Trie树，或者我在网上能够
找到的开源实现，大多是只要字符串里面有这个pattern就算作匹配，而不是从头进行
匹配。举例说明：当插入ab到Trie后，"dab”和"abc"是可以匹配的。但使用ExactTrie
执行的是精准匹配，所以"dab"和"abc"是不匹配的。当进行前缀匹配时，只有"abc"可
以匹配"ab"。

因此我就自己实现了这个ExactTrie。
1. 支持精准匹配；  
2. 支持前缀匹配；  
3. 支持连续匹配，即上一次的匹配结果继续匹配；  
4. 支持返回pattern的指定data；  


# General  

When need to match the exact strings or exact prefixs, the original solution
is that put the strins into different buckets by hash. When lookup, hash the
string firstly, then find one bucket, and compare the string with every pattern
in this bucket. 

When there are many similar patterns, there may be lots of patterns in the same
bucket. It would cost lot of time to match.

The trie is good at resolving this problem. But most of original tries perform
the substring match, not from the beginning of string. For example, when insert
"ab" into Trie, the "dab" and "abc" could be matched. But ExactTrie performs the
exact match, so fails to match them. When do the prefix match of ExactTrie, it 
could match the "ab".

So I build this project "ExactTrie".  
1. Support exact match;  
2. Support prefix match;  
3. Support continous match, continue searching from the last match pos;  
4. Support return the patter's data;  


