#!/bin/bash

HOST="dvd2db"
DATABASE="dvd2db"
USERNAME="root"

mysql --ssl=0 -h $HOST -u $USERNAME $DATABASE << EOF
DELETE FROM customers
WHERE USERNAME LIKE 'temp%';
EOF

echo "DVD2 database reset successfully."
