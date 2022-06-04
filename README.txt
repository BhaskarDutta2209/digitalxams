# Project: digitalxams

## What it does ?
A blockchain based replacement for evaluating exams

## Details:
Build with: <code>eosio-cpp</code>
Target blockchain: <code>TELOS</code>

## To compile
```
eosio-cpp -abigen -I include -R resource -contract digitalxams -o digitalxams.wasm src/digitalxams.cpp
```

## To deploy
```
cleos -u https://testnet.telos.caleos.io:443 set contract digitalxams ./ -p digitalxams@active
```