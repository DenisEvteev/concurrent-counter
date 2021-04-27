#!/bin/bash

echo "###run GenMC model checker###"

GenMC='genmc/src/genmc'
$GenMC -- -pthread test.c