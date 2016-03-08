#!/bin/bash

# Exit the script if any statement returns a non-true return value
set -e

# Compile universal-c sdk
cd ../..
scripts/deps.sh
make