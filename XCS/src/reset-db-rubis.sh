#!/bin/bash

HOST="rubisdb1"
DATABASE="rubis"
USERNAME="root"

#echo "Resetting RUBiS database..."

# Use --ssl=0 for older MySQL clients
mysql --ssl=0 -h $HOST -u $USERNAME $DATABASE << EOF
DELETE FROM users WHERE id <> 32;
DELETE FROM comments;
DELETE FROM items WHERE id <> 10490;
DELETE FROM bids;
EOF

#if [ $? -eq 0 ]; then
#    echo "Database reset successful!"
#else
#    echo "Database reset failed!"
#    exit 1
#fi