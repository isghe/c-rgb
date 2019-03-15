#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <rgb.h>
#include "lib/rgb_example_util.h"

static inline uint8_t char_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 0x0A;
    return c - 'A' + 0x0A;
}

void decode_hex(const char *c, uint8_t *buffer, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        buffer[i] = (char_value(c[i * 2]) << 4) | char_value(c[i * 2 + 1]);
    }
}

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
    struct rgb_bitcoin_serialized_tx tx = {
            .size = (sizeof(hex) / 2),
            .payload = (uint8_t *) malloc(sizeof(hex) / 2)
    };

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
    return EXIT_SUCCESS;
}
