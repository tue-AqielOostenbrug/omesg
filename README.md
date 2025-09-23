# qmesg - opensource IRCv3 messaging
## Project structure
```
.
├── Makefile
├── README.md
├── lib
└── src
    ├── esp32
    │   ├── client
    │   │   ├── Makefile
    │   │   └── client.c
    │   ├── lora
    │   │   ├── Makefile
    │   │   └── client.c
    │   └── server
    │       ├── Makefile
    │       └── server.c
    └── linux
        ├── client
        │   ├── Dockerfile
        │   ├── Makefile
        │   └── client.c
        └── server
            ├── Makefile
            └── server.c
```
## Goals
- [M] Prototype Linux client (Under constr.)
- [S] Unit test Linux client
- [S] Integrate Linux client into Docker testing pipeline (Under constr.)
- [M] Prototype ESP32 client (Under constr.)
- [S] Unit test ESP32 client
- [S] Integrate ESP32 client into Docker testing pipeline
- [C] Prototype Linux server
- [C] Unit test Linux server
- [C] Integrate Linux server into Docker testing pipeline
- [C] Prototype ESP32 server
- [C] Unit test ESP32 server
- [C] Integrate ESP32 server into Docker testing pipeline
- [C] Integrate LORA into ESP32 client
- [C] Integrate LORA into ESP32 server

## Known issues
*Linux Client:*
- Socket buffer size mismatch leading to parsing issues
- Unsafe parsing
