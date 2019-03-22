#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rgb.h>
#include "lib/rgb_example_util.h"

int main() {
/*
credo che il problema nel tuo verify_contract è che issuance_utxo deve rappresentare
l'utxo *speso* nella tx di issuance, non quello creato. ho notato che il txid è lo stesso
della tx serializzata che c'è nel codice

$ ./target/debug/kaleidoscope issueasset --network regtest --title "My Awesome Asset" --supply 1000
Asset ID: 45d0658000e70f77b805be758921010cb0a5bc07e537d6128080ba3328d8a7df
Spending the issuance_utxo e9951a3dc28ecfd98efc520c14c391a0a309e8123b299b429dd8dbe63e2d7946:0 in 570684e4301bcc9b6f6180fe7dfba10ed9f9543b110606d4e1d2d75977b86a93
Spending the initial_owner_utxo 4a8c0c170873c5b0be329fc6842dec3b8aed60bc40e90f48c0efec44255c86c3:0 in 926161d5f3209f42b5b2dd9e7577c50b068f90a0eb1681c94cd772a8f25fac8f

*/
    struct rgb_bitcoin_outpoint issuance_utxo = {
            .vout = 0
    };

    const size_t testIndex = 2;
    const char * issuance_utxos [] = {
		"08a8efe33fd3dda4f878f995d843bd70471a8a32c83c3a9418a09ca30759cb02",
		"e9951a3dc28ecfd98efc520c14c391a0a309e8123b299b429dd8dbe63e2d7946",
		"46792d3ee6dbd89d429b293b12e809a3a091c3140c52fc8ed9cf8ec23d1a95e9"
    };
	HexToBin (issuance_utxos [testIndex], (unsigned char *) &issuance_utxo.txid.val, 0);

    struct rgb_bitcoin_outpoint initial_owner_utxo = {
            .vout = 0
    };

    const char * initial_owner_utxos  [] = {
		"54a56e95e583fa11e93b9c7ccbd933eaa71e6eaeab376969a7eb2735ee84feb5",
		"4a8c0c170873c5b0be329fc6842dec3b8aed60bc40e90f48c0efec44255c86c3",
		"c3865c2544ecefc0480fe940bc60ed8a3bec2d84c69f32beb0c57308170c8c4a"
    };

	HexToBin (initial_owner_utxos [testIndex], (unsigned char *) &initial_owner_utxo.txid.val, 0);

    struct rgb_contract contract = {
            .title = "My Awesome Asset",
            .issuance_utxo = issuance_utxo,
            .initial_owner_utxo = initial_owner_utxo,
            .network = RGB_NETWORK_TESTNET,
            .total_supply = 1000
    };

    // hex of issuance_utxo tx
    const char * hexs [] = {
		"020000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff0502e9010101ffffffff0240be40250000000023210314ee885fe5705d7b789b1dea86ce5797500192633c2f07e517fefbfb529152acac0000000000000000266a24aa21a9ede2f61c3f71d1defd3fa999dfa36953755c690689799962b48bebd836974e8cf90120000000000000000000000000000000000000000000000000000000000000000000000000",
		"010000000146792d3ee6dbd89d429b293b12e809a3a091c3140c52fc8ed9cf8ec23d1a95e9000000004847304402205d68b44e42b47a7cc3b4800d100e1a99549bfa30ca0d1277490054c696e0f62002204eff912bb8841c5705ba6d4c95f3d757c6dab9fcd82d5f378ab6d95c7cfba07f01000000000248e6052a0100000017a9148f7cc036b8b011b8343f685cd19f7456950316c2870000000000000000226a20dfa7d82833ba808012d637e507bca5b00c01218975be05b8770fe7008065d04500000000",
		"010000000146792d3ee6dbd89d429b293b12e809a3a091c3140c52fc8ed9cf8ec23d1a95e9000000004847304402205d68b44e42b47a7cc3b4800d100e1a99549bfa30ca0d1277490054c696e0f62002204eff912bb8841c5705ba6d4c95f3d757c6dab9fcd82d5f378ab6d95c7cfba07f01000000000248e6052a0100000017a9148f7cc036b8b011b8343f685cd19f7456950316c2870000000000000000226a20dfa7d82833ba808012d637e507bca5b00c01218975be05b8770fe7008065d04500000000"
    };
    const char * hex = hexs [testIndex];
    const size_t hex_length = strlen (hex);
    struct rgb_bitcoin_serialized_tx tx = {
            .size = (hex_length / 2),
            .payload = (uint8_t *) malloc(hex_length / 2)
    };

	HexToBin(hex, tx.payload, 0);

    const struct rgb_needed_tx need = {
            .type = RGB_NEEDED_TX_SPENDS_OUTPOINT,
            .outpoint = issuance_utxo
    };

    struct rgb_needed_tx_map *map = rgb_init_needed_tx_map();
    rgb_push_needed_tx_map(map, &need, &tx);

    // The check will fail with "invalid commitment"
    uint8_t result = rgb_contract_verify(&contract, map);

    printf("Verification result: %u\n", result);

    rgb_free(map,
    struct rgb_needed_tx_map);
	free (tx.payload);
	tx.payload = NULL;
	tx.size = 0;
    return EXIT_SUCCESS;
}
