#!/bin/bash

# Zoek naar bestanden genaamd 'fp-info-cache' of eindigend op '.kicad_sch.lck' of '.kicad_pcb.lck' en verwijder ze.

find . -type f \( -name "fp-info-cache" -o -name "*.kicad_sch.lck" -o -name "*.kicad_pcb.lck" \) -delete