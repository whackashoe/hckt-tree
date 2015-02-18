hckt-tree
===

HexaContaKaiteTratet Tree

This a tree data structure in which each node has up to 64 children. What is interesting about this configuration is we are able to use the same tree optimization in both 2d (through 3 levels) and 3d (through 2 levels).

This implementation uses a 64 long bitset to encode which subtrees exist, drastically cutting down the amount of necessary memory normally needed in quad or octrees at the expense of extra computational time for the lookup. However, the improvements to locality in addition to less pointer indirection may offset this to a significant degree.

MIT licensed for love
