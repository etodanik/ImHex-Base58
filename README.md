# ImHex Base58 Plugin

Adds a few useful features for working with Bitcoin style Base58 values (useful to debug Solana and Ethereum transactions and data structures).

## Inline Visualizer

By adding a `base58` inline visualizer it is possible to display encoded base58 string values for byte arrays.
```
struct PublicKey {
    u8 data[32];
} [[hex::inline_visualize("base58", data)]];
```

![visualizer](https://github.com/user-attachments/assets/7284b878-be47-4ea0-adb5-26ca07da2043)

## Node

There is also a data processing node that can decode a base58 string to a raw byte array.

![node](https://github.com/user-attachments/assets/cfd7b637-5731-4e36-a06e-12ace902f9e3)
