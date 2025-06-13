#!/bin/bash

# Zoek naar ongewenste bestanden en verwijder ze:
# - 'fp-info-cache'
# - '*.kicad_sch.lck'
# - '*.kicad_pcb.lck'
# - '*.gbr'
# - '*.drl'
# - '*.lck'

find . -type f \( \
  -name "fp-info-cache" \
  -o -name "*.kicad_sch.lck" \
  -o -name "*.kicad_pcb.lck" \
  -o -name "*.gbr" \
  -o -name "*.drl" \
  -o -name "*.lck" \
\) -exec rm -v {} +
