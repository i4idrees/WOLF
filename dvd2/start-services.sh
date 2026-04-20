#!/bin/bash

# Start MySQL service
service mysql start

# Wait for MySQL to be ready
while ! mysqladmin ping -hlocalhost --silent; do
    sleep 1
done

# Initialize database using DVD2DB.sql (your actual file name)
if [ -f /docker-entrypoint-initdb.d/DVD2DB.sql ]; then
    mysql -u root < /docker-entrypoint-initdb.d/DVD2DB.sql
fi

# Start Apache in foreground
exec apache2-foreground