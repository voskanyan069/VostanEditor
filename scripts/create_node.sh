#!/bin/bash

curl --data "$1" -H "Metax-Content-Type:application/json" "http://localhost:8001/db/save/node?enc=0&local=1"
echo ""
