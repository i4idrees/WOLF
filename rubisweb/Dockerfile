FROM php:7.4-apache

# Install PDO and PDO MySQL extensions
RUN docker-php-ext-install pdo pdo_mysql

# Copy web content to /var/www/html
COPY ./htdocs /var/www/html

# Expose port 80
EXPOSE 80

# Start Apache in the foreground
CMD ["apache2-foreground"]
