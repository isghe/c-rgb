#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include <rgb.h>
#include "lib/rgb_example_util.h"

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

    struct rgb_sha256d *asset_id = rgb_contract_get_asset_id(&contract);

    struct rgb_output_entry entry = {
            .asset_id = *asset_id,
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

    struct rgb_allocated_array_uint8_t serialized_proof = rgb_proof_serialize(&proof);

    printf("Proof (hex): ");
    print_hex(serialized_proof.ptr, serialized_proof.size);

    struct rgb_proof *deserialized_proof = rgb_proof_deserialize(serialized_proof.ptr, serialized_proof.size);

    rgb_debug_print_proof(deserialized_proof);

    rgb_free(asset_id,
    struct rgb_sha256d);
    rgb_free(deserialized_proof,
    struct rgb_proof);
    rgb_free_array(serialized_proof,
    struct rgb_allocated_array_uint8_t);

    return EXIT_SUCCESS;
}