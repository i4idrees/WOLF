echo "<?php
try {
    // Database connection details
    \$host = 'mysql-db';
    \$username = 'root';
    \$password = '123';
    \$database = 'rubis';

    // Create a new PDO connection
    \$conn = new PDO('mysql:host=' . \$host . ';dbname=' . \$database, \$username, \$password);
    // Set the PDO error mode to exception
    \$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    echo 'Connection successful!';
} catch (PDOException \$e) {
    echo 'Connection failed: ' . \$e->getMessage();
}
" > test_connection.php
