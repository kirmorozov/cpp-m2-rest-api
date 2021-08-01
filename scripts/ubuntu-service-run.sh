#!/bin/bash

build/mg_m2_api config.example.json &
echo $! > process.pid

