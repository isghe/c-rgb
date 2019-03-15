#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <rgb.h>
#include "lib/rgb_example_util.h"

int main() {
    struct rgb_bitcoin_outpoint issuance_utxo = {
            .txid = {{0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}},
            .vout = 3
    };

    struct rgb_bitcoin_outpoint initial_owner_utxo = {
            .txid = {{0x00, 0x01, 0x02, 0x04, 0x08, 0xFF}},
            .vout = 0
    };

    struct rgb_contract contract = {
            .title = "My Title",
            .issuance_utxo = issuance_utxo,
            .initial_owner_utxo = initial_owner_utxo,
            .network = RGB_NETWORK_TESTNET,
            .total_supply = 1000
    };

    rgb_debug_print_contract(&contract);

    struct rgb_allocated_array_rgb_needed_tx needed_txs = rgb_contract_get_needed_txs(&contract);

    printf("Number of elements: %lu\n", needed_txs.len);

    for (size_t i = 0; i < needed_txs.len; ++i) {
        if (needed_txs.ptr[i].type == RGB_NEEDED_TX_TXID) {
            printf("FromTXID(\n\ttxid: ");
            print_hex((void *) &needed_txs.ptr[i].txid, 32);
            printf("\n)\n");

        } else if (needed_txs.ptr[i].type == RGB_NEEDED_TX_SPENDS_OUTPOINT) {
            printf("WhichSpendsOutPoint(\n\ttxid: ");
            print_hex((void *) &needed_txs.ptr[i].outpoint.txid, 32);
            printf("\n\tvout: %u\n)\n", needed_txs.ptr[i].outpoint.vout);

        } else {
            fprintf(stderr, "Unknown NeededTx type %u. Exiting now.\n", needed_txs.ptr[i].type);

            return EXIT_FAILURE;
        }
    }

    rgb_free_array(needed_txs,
    struct rgb_allocated_array_rgb_needed_tx);

    return EXIT_SUCCESS;
}