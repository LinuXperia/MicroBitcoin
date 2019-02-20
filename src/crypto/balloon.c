// Copyright (c) 2018 iamstenman
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// #include "sphlib/sph_groestl.h"
#include "sphlib/sph_shabal.h"
#include <memory.h>
#include <inttypes.h>

// Convert uint8_t to uint64_t
uint64_t u8tou64(uint8_t const *u8){
	uint64_t u64;
	memcpy(&u64, u8, sizeof(u64));
	return u64;
}

// Any arbitrary data can be used as salt
void getsalt(const void* input, unsigned char* salt) {
	// Copy previous block hash from block header
	memcpy(salt, input + 4, 32 * sizeof(*input));
}

// Return pointer to the block by index
void *block_index(uint8_t *blocks, size_t i) {
	return blocks + (64 * i);
}

void balloon(const void *input, void *output, int length)
{
	const uint64_t s_cost = 16;
	const uint64_t t_cost = 2;
	const int delta = 3;
	
	sph_shabal512_context context;
	uint8_t blocks[s_cost * 64];
	uint64_t cnt = 0;

	const int slength = 32;
	uint8_t salt[slength];
	getsalt(input, salt);
	
	// Step 1: Expand input into buffer
	sph_shabal512_init(&context);
	sph_shabal512(&context, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
	sph_shabal512(&context, input, length);
	sph_shabal512(&context, salt, slength);
	sph_shabal512_close(&context, block_index(blocks, 0));
	cnt++;

	for (int m = 1; m < s_cost; m++) {
		sph_shabal512(&context, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
		sph_shabal512(&context, block_index(blocks, m - 1), 64);
		sph_shabal512_close(&context, block_index(blocks, m));
		cnt++;
	}

	// Step 2: Mix buffer contents
	for (uint64_t t = 0; t < t_cost; t++) {
		for (uint64_t m = 0; m < s_cost; m++) {
			// Step 2a: Hash last and current blocks
			sph_shabal512(&context, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
			sph_shabal512(&context, block_index(blocks, (m - 1) % s_cost), 64);
			sph_shabal512(&context, block_index(blocks, m), 64);
			sph_shabal512_close(&context, block_index(blocks, m));
			cnt++;

			for (uint64_t i = 0; i < delta; i++) {
				// Step 2b: Hash in pseudorandomly chosen blocks
				uint8_t index[64];
				sph_shabal512(&context, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
				sph_shabal512(&context, (uint8_t *)&t, sizeof((uint8_t *)&t));
				sph_shabal512(&context, (uint8_t *)&m, sizeof((uint8_t *)&m));
				sph_shabal512(&context, (uint8_t *)&i, sizeof((uint8_t *)&i));
				sph_shabal512(&context, salt, slength);
				sph_shabal512_close(&context, index);
				cnt++;

				uint64_t other = u8tou64(index) % s_cost;
				sph_shabal512(&context, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
				sph_shabal512(&context, block_index(blocks, m), 64);
				sph_shabal512(&context, block_index(blocks, other), 64);
				sph_shabal512_close(&context, block_index(blocks, m));
				cnt++;
			}
		}
	}
	
	memcpy(output, block_index(blocks, s_cost - 1), 32);
}
