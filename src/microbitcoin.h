#ifndef MICROBITCOIN_H
#define MICROBITCOIN_H

/** Get block height */
int GetBlockHeight(const CBlockHeader &block);
int GetBlockHeight(const uint256 &hashPrevBlock);

#endif // MICROBITCOIN_H