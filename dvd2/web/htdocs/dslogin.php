<?php
/*
 * DVD Store Login PHP Page - dslogin.php
 * Updated for PHP 7.4+ (uses mysqli)
 */

include("dscommon.inc");

ds_html_header("DVD Store Login Page");

$username = $_REQUEST["username"] ?? '';
$password = $_REQUEST["password"] ?? '';

if (!empty($username)) {
  // Connect to MySQL
  $link = new mysqli("dvd2db", "root", "", "dvd2db", 3306); // Change port if needed

  if ($link->connect_error) {
    die("Connection failed: " . $link->connect_error);
  }

  // Escape input to prevent SQL injection
  $username_escaped = $link->real_escape_string($username);
  $password_escaped = $link->real_escape_string($password);

  // Query to verify user
  $query = "SELECT CUSTOMERID FROM customers WHERE USERNAME='$username_escaped' AND PASSWORD='$password_escaped'";
  $result = $link->query($query);
  if ($result && $result->num_rows > 0) {
    $row = $result->fetch_row();
    $customerid = $row[0];
    $result->free();

    echo "<h2>Welcome to the DVD Store - Click below to begin shopping</h2>\n";

    // Get previous purchases
    $query = "SELECT P.TITLE, P.ACTOR, P.COMMON_PROD_ID
                  FROM CUST_HIST CH
                  INNER JOIN PRODUCTS P ON CH.PROD_ID = P.PROD_ID
                  WHERE CH.CUSTOMERID = $customerid
                  ORDER BY ORDERID DESC, TITLE ASC
                  LIMIT 10";

    $result = $link->query($query);

    if ($result && $result->num_rows > 0) {
      echo "<h3>Your previous purchases:</h3>\n";
      echo "<table border=2>\n<tr><th>Title</th><th>Actor</th><th>People who liked this DVD also liked</th></tr>\n";

      while ($result_row = $result->fetch_row()) {
        $common_id = (int)$result_row[2];
        $query2 = "SELECT TITLE FROM PRODUCTS WHERE PROD_ID = $common_id";
        $result2 = $link->query($query2);
        $row2 = $result2 ? $result2->fetch_row() : [""];
        $result2 && $result2->free();

        echo "<tr><td>{$result_row[0]}</td><td>{$result_row[1]}</td><td>{$row2[0]}</td></tr>\n";
      }

      echo "</table><br>\n";
      $result->free();
    }

    echo "<form action=\"./dsbrowse.php\" method=\"get\">
                <input type=\"hidden\" name=\"customerid\" value=\"$customerid\">
                <input type=\"submit\" value=\"Start Shopping\">
              </form>\n";
  } else {
    // Login failed
    echo "<h2>Username/password incorrect. Please re-enter your username and password</h2>\n";
    echo "<form action=\"./dslogin.php\" method=\"get\">
                Username <input type=\"text\" name=\"username\" value=\"$username\" size=\"16\" maxlength=\"24\">
                Password <input type=\"password\" name=\"password\" size=\"16\" maxlength=\"24\">
                <input type=\"submit\" value=\"Login\">
              </form>
              <h2>New customer? Please click New Customer</h2>
              <form action=\"./dsnewcustomer.php\" method=\"get\">
                <input type=\"submit\" value=\"New Customer\">
              </form>\n";
  }

  $link->close();
} else {
  // Initial login form
  echo <<<EOT
<h2>Returning customer? Please enter your username and password</h2>
<form action="./dslogin.php" method="get">
  Username <input type="text" name="username" size="16" maxlength="24">
  Password <input type="password" name="password" size="16" maxlength="24">
  <input type="submit" value="Login">
</form>
<h2>New customer? Please click New Customer</h2>
<form action="./dsnewcustomer.php" method="get">
  <input type="submit" value="New Customer">
</form>
EOT;
}

ds_html_footer();
?>
