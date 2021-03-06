#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include <rgb.h>

void print_hex(const void *p, size_t len) {
    for (size_t i = 0; i < len; ++i)
        printf("%02hhx", *((uint8_t *) p + i));

    printf("\n");
}

int main() {
    struct rgb_bitcoin_outpoint utxo = {
            .txid = {{0xDE, 0xAD, 0xBE, 0xEF, 0xFF}},
            .vout = 3
    };

    struct rgb_contract contract = {
            .title = "My Title",
            .issuance_utxo = utxo,
            .initial_owner_utxo = utxo,
            .network = RGB_NETWORK_TESTNET,
            .total_supply = 1000
    };

    struct rgb_sha256d asset_id;
    rgb_contract_get_asset_id(&contract, &asset_id);

    struct rgb_output_entry entry = {
            .asset_id = asset_id,
            .amount = contract.total_supply,
            .vout = 0
    };

    struct rgb_proof proof = {
            .bind_to_count = 1,
            .bind_to = &utxo,
            .input_count = 0,
            .input = NULL,
            .output_count = 1,
            .output = &entry,
            .contract = &contract
    };

    rgb_debug_print_proof(&proof);

    uint32_t size = rgb_proof_get_serialized_size(&proof);

    uint8_t *buffer = malloc(size);
    uint32_t size_2 = rgb_proof_serialize(&proof, &buffer);

    assert(size == size_2);

    printf("Proof (hex): ");
    print_hex(buffer, size);

    struct rgb_proof deserialized_proof;

    rgb_proof_deserialize(buffer, size, &deserialized_proof);

    rgb_debug_print_proof(&deserialized_proof);

    return EXIT_SUCCESS;
}