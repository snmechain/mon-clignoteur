# mon clignoteur GPIO21

[![C CI Compilation](https://github.com/snmechain/mon-clignoteur/actions/workflows/ci.yml/badge.svg)](https://github.com/snmechain/mon-clignoteur/actions/workflows/ci.yml)

Ce projet est un clignoteur aléatoire pour Raspberry Pi utilisant la bibliothèque `libgpiod`. 
Il fait clignoter une LED sur le GPIO 21 à un rythme qui change toutes les 10 secondes.

## Compilation locale
```bash
make
sudo ./clignoteur
