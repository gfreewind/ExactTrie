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

# Performance

There are two kinds of Performance Tests.
One is done between ExactTrie and IterMemcmp, another is performed between ExactTrie and HashMemcmp.

The ExactTrie is better than IterMemcmp very much, and it could be good like HashMemcmp. 
When the hash is not averaged, it would better than HashMemcmp too.  

## Concolusion 

The cost of ExactTrie match is almost not changed with increasing of the count and length of patterns.
 

## ExactTrie vs IterMemcmp  


The following is the result of performance test:

Insert 8 random strings(length is 1):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 36000000 times, cost 50 secs  
ExactTrie match 36000000 times, cost 70 secs  

Insert 16 random strings(length is 1):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 67000000 times, cost 95 secs  
ExactTrie match 67000000 times, cost 77 secs  

Insert 32 random strings(length is 1):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 130000000 times, cost 192 secs  
ExactTrie match 130000000 times, cost 81 secs  

Insert 8 random strings(length is 2):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 64 secs  
ExactTrie match 0 times, cost 70 secs  

Insert 16 random strings(length is 2):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 138 secs  
ExactTrie match 0 times, cost 76 secs  

Insert 32 random strings(length is 2):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 1000000 times, cost 261 secs  
ExactTrie match 1000000 times, cost 81 secs  

Insert 8 random strings(length is 4):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 75 secs  
ExactTrie match 0 times, cost 70 secs  

Insert 16 random strings(length is 4):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 157 secs  
ExactTrie match 0 times, cost 77 secs  

Insert 32 random strings(length is 4):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 301 secs  
ExactTrie match 0 times, cost 81 secs  

Insert 64 random strings(length is 4):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 590 secs  
ExactTrie match 0 times, cost 87 secs  

Insert 128 random strings(length is 4):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 1170 secs  
ExactTrie match 0 times, cost 94 secs  

Insert 8 random strings(length is 8):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 81 secs  
ExactTrie match 0 times, cost 71 secs  

Insert 16 random strings(length is 8):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 172 secs  
ExactTrie match 0 times, cost 75 secs  

Insert 32 random strings(length is 8):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 329 secs  
ExactTrie match 0 times, cost 82 secs  

Insert 64 random strings(length is 8):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 645 secs  
ExactTrie match 0 times, cost 87 secs  
              
Insert 128 random strings(length is 8):  
Lookup 1024 random strings(loop 1000000 times):  
IterMemcmp match 0 times, cost 1276 secs  
ExactTrie match 0 times, cost 93 secs  


## ExactTrie vs HashMemcmp (Hash firstly, then memcmp)  

Perf Test: ExactTrie with HashMemcmp  
Insert 64 random strings(length is 2):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 10 secs  
ExactTrie match 0 times, cost 8 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 128 random strings(length is 2):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 10 secs  
ExactTrie match 0 times, cost 10 secs  

Perf Test: ExactTrie with HashMemcmp  
Insert 256 random strings(length is 2):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 10 secs  
ExactTrie match 0 times, cost 9 secs  

Perf Test: ExactTrie with HashMemcmp  
Insert 512 random strings(length is 2):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 10 secs  
ExactTrie match 0 times, cost 10 secs  

Perf Test: ExactTrie with HashMemcmp  
Insert 64 random strings(length is 4):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 11 secs  
ExactTrie match 0 times, cost 8 secs  

Perf Test: ExactTrie with HashMemcmp  
Insert 128 random strings(length is 4):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 10 secs  
ExactTrie match 0 times, cost 9 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 256 random strings(length is 4):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 9 secs  
ExactTrie match 0 times, cost 10 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 512 random strings(length is 4):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 11 secs  
ExactTrie match 0 times, cost 10 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 64 random strings(length is 8):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 15 secs  
ExactTrie match 0 times, cost 7 secs  

Perf Test: ExactTrie with HashMemcmp  
Insert 128 random strings(length is 8):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 13 secs  
ExactTrie match 0 times, cost 8 secs  

Perf Test: ExactTrie with HashMemcmp  
Insert 256 random strings(length is 8):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 10 secs  
ExactTrie match 0 times, cost 9 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 512 random strings(length is 8):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 11 secs  
ExactTrie match 0 times, cost 12 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 64 random strings(length is 16):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 18 secs  
ExactTrie match 0 times, cost 8 secs  

Perf Test: ExactTrie with HashMemcmp  
Insert 128 random strings(length is 16):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 14 secs  
ExactTrie match 0 times, cost 9 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 256 random strings(length is 16):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 15 secs  
ExactTrie match 0 times, cost 10 secs  


Perf Test: ExactTrie with HashMemcmp  
Insert 512 random strings(length is 16):  
Lookup 1024 random strings(loop 100000 times):  
HashMemcmp match 0 times, cost 15 secs  
ExactTrie match 0 times, cost 10 secs  



