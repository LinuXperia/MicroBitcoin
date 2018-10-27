// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <validation.h>
#include <net.h>

#include <test/test_bitcoin.h>

#include <boost/signals2/signal.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(main_tests, TestingSetup)

static int halvingsHeight(int nHalvings, const Consensus::Params& consensusParams) {
    if (nHalvings < 3) {
        return nHalvings * consensusParams.nSubsidyHalvingInterval;
    } else if (nHalvings == 3) {
        return 1574991;
    } else {
        return 1574991 + (nHalvings - 3) * consensusParams.nSubsidyHalvingInterval * 10;
    }
}

static void TestBlockSubsidyHalvings(const Consensus::Params& consensusParams)
{
    int maxHalvings = 2 + 18;
    CAmount nInitialSubsidy = 50 * COIN * COIN_RATIO;

    CAmount nPreviousSubsidy = nInitialSubsidy * 2;
    BOOST_CHECK_EQUAL(nPreviousSubsidy, nInitialSubsidy * 2);

    for (int nHalvings = 0; nHalvings < maxHalvings; nHalvings++) {
        const int nHeight = halvingsHeight(nHalvings, consensusParams);

        CAmount nSubsidy = GetBlockSubsidy(nHeight, consensusParams);
        if (consensusParams.mbcHeight <= nHeight) {
            nSubsidy = nSubsidy * ((consensusParams.nSubsidyHalvingInterval * 10) / consensusParams.nSubsidyHalvingInterval);
            BOOST_CHECK((nSubsidy == nPreviousSubsidy / 2) || ((nSubsidy + 5) == nPreviousSubsidy / 2));
        } else {
            BOOST_CHECK_EQUAL(nSubsidy, nPreviousSubsidy / 2);
        }

        BOOST_CHECK(nSubsidy <= nInitialSubsidy);
        nPreviousSubsidy = nSubsidy;
    }
    BOOST_CHECK_EQUAL(GetBlockSubsidy(halvingsHeight(maxHalvings, consensusParams), consensusParams), 0);
}

BOOST_AUTO_TEST_CASE(block_subsidy_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    TestBlockSubsidyHalvings(chainParams->GetConsensus()); // As in main
}

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    CAmount nSum = 0;
    for (int nHeight = 0; nHeight < 14000000; nHeight += 1000) {
        CAmount nSubsidy = GetBlockSubsidy(nHeight, chainParams->GetConsensus());
        BOOST_CHECK(nSubsidy <= 50 * COIN * COIN_RATIO);
        nSum += nSubsidy * 1000;
        BOOST_CHECK(MoneyRange(nSum));
    }
    BOOST_CHECK_EQUAL(nSum, 2096681640625000ULL);
}

bool ReturnFalse() { return false; }
bool ReturnTrue() { return true; }

BOOST_AUTO_TEST_CASE(test_combiner_all)
{
    boost::signals2::signal<bool (), CombinerAll> Test;
    BOOST_CHECK(Test());
    Test.connect(&ReturnFalse);
    BOOST_CHECK(!Test());
    Test.connect(&ReturnTrue);
    BOOST_CHECK(!Test());
    Test.disconnect(&ReturnFalse);
    BOOST_CHECK(Test());
    Test.disconnect(&ReturnTrue);
    BOOST_CHECK(Test());
}
BOOST_AUTO_TEST_SUITE_END()