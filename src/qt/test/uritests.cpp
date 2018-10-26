// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/test/uritests.h>

#include <qt/guiutil.h>
#include <qt/walletmodel.h>

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?label=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?amount=0.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 10);

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?amount=1.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 10010);

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?amount=100&label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW"));
    QVERIFY(rv.amount == 1000000LL);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseBitcoinURI("microbitcoin://BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?message=Wikipedia Example Address", &rv));
    QVERIFY(rv.address == QString("BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?req-message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?amount=1,000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("microbitcoin:BjDuZq9ut9JiZzy6VozRnCzCjHfFrSYobW?amount=1,000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));
}
