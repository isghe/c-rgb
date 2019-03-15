#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rgb.h>
#include "lib/rgb_example_util.h"

int main() {

    struct rgb_bitcoin_outpoint issuance_utxo = {
            .vout = 3
    };
	HexToBin ("08a8efe33fd3dda4f878f995d843bd70471a8a32c83c3a9418a09ca30759cb02", (unsigned char *) &issuance_utxo.txid.val);

    struct rgb_bitcoin_outpoint initial_owner_utxo = {
            .vout = 0
    };
	HexToBin ("54a56e95e583fa11e93b9c7ccbd933eaa71e6eaeab376969a7eb2735ee84feb5", (unsigned char *) &initial_owner_utxo.txid.val);

    struct rgb_contract contract = {
            .title = "My Title",
            .issuance_utxo = issuance_utxo,
            .initial_owner_utxo = initial_owner_utxo,
            .network = RGB_NETWORK_TESTNET,
            .total_supply = 1000
    };

    // hex of issuance_utxo tx
    const char hex[] = "020000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff0502e9010101ffffffff0240be40250000000023210314ee885fe5705d7b789b1dea86ce5797500192633c2f07e517fefbfb529152acac0000000000000000266a24aa21a9ede2f61c3f71d1defd3fa999dfa36953755c690689799962b48bebd836974e8cf90120000000000000000000000000000000000000000000000000000000000000000000000000";
    const size_t hex_length = sizeof (hex) - 1; //  sizeof (hex) - 1 == strlen (hex);
    struct rgb_bitcoin_serialized_tx tx = {
            .size = (hex_length / 2),
            .payload = (uint8_t *) malloc(hex_length / 2)
    };

	HexToBin(hex, tx.payload);

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
