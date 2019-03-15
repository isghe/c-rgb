#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

    rgb_debug_print_contract(&contract);

    struct rgb_sha256d *asset_id = rgb_contract_get_asset_id(&contract);

    printf("From Rust: ");
    fflush(stdout); // without this the order of prints would likely be messed up
    rgb_debug_sha256d(asset_id);

    printf("From C:    ");
    print_hex((const void *) asset_id, 32);

    printf("\n");

    // Let's ask Rust for the commitment script
    struct rgb_allocated_array_uint8_t script = rgb_contract_get_expected_script(&contract);

    printf("Expected commitment output: ");
    print_hex((const void *) script.ptr, script.size);

    printf("\n");

    // And now we serialize it
    struct rgb_allocated_array_uint8_t serialized_contract = rgb_contract_serialize(&contract);

    printf("Serialized contract: ");
    print_hex((const void *) serialized_contract.ptr, serialized_contract.size);

    printf("\n");

    // And deserialize it
    struct rgb_contract *deserialized_contract = rgb_contract_deserialize(serialized_contract.ptr,
									  serialized_contract.size);

    rgb_debug_print_contract(deserialized_contract);

    rgb_free(asset_id,
    struct rgb_sha256d);
    rgb_free(deserialized_contract,
    struct rgb_contract);
    rgb_free_array(script,
    struct rgb_allocated_array_uint8_t);
    rgb_free_array(serialized_contract,
    struct rgb_allocated_array_uint8_t);

    return EXIT_SUCCESS;
}