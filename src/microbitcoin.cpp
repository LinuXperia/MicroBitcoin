#include <chain.h>
#include <microbitcoin.h>
#include <validation.h>

/** Get block height */

int GetBlockHeight(const CBlockHeader &block) {
    return GetBlockHeight(block.hashPrevBlock);
}

int GetBlockHeight(const uint256 &hashPrevBlock) {
    CBlockIndex *pindexPrev = NULL;
    int nHeight = 0;

    BlockMap::iterator mi = mapBlockIndex.find(hashPrevBlock);
    if (mi != mapBlockIndex.end()) {
        pindexPrev = (*mi).second;
        nHeight = pindexPrev->nHeight + 1;
    }

    return nHeight;
}