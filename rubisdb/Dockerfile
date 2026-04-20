FROM mysql:5.7

# Set environment variables for MySQL (adjust if necessary)
ENV MYSQL_ALLOW_EMPTY_PASSWORD=yes
ENV MYSQL_DATABASE=rubis

# Copy the SQL file to the Docker entrypoint directory
COPY rubisDB.sql /docker-entrypoint-initdb.d/

#Newly added line for performance database
COPY perfDB.sql /docker-entrypoint-initdb.d/

# Expose port 3306
EXPOSE 3306
