# ExactTrie

It is different with the original trie.   
Take an example, when insert "abc" into the trie.  
The original trie could match the "ab" or "dabc", but this exact trie failed.
It only match the exact string inserted.

When insert "ab" and "abc" into the exact trie, it could match both of them.
