<?php
/*  
 * DVD Store Browse PHP Page - dsbrowse.php
 * Updated for PHP 7.4+ (mysqli-compatible, safe count(), null handling)
 */

include("dscommon.inc");

ds_html_header("DVD Store Browse Page");

$customerid = $_REQUEST["customerid"] ?? '';
$browsetype = $_REQUEST["browsetype"] ?? null;
$browse_title = $_REQUEST["browse_title"] ?? null;
$browse_actor = $_REQUEST["browse_actor"] ?? null;
$browse_category = $_REQUEST["browse_category"] ?? null;
$limit_num = $_REQUEST["limit_num"] ?? 10;
$selected_item = $_REQUEST["selected_item"] ?? [];
$item = $_REQUEST["item"] ?? [];

// Redirect if not logged in
if (empty($customerid)) {
  echo "<h2>You have not logged in - Please click below to Login to DVD Store</h2>\n";
  echo "<form action='./dslogin.php' method='get'>\n";
  echo "<input type='submit' value='Login'>\n";
  echo "</form>\n";
  ds_html_footer();
  exit;
}

// Add new items to shopping cart
if (!empty($selected_item)) {
  $n_items = is_array($item) ? count($item) : 0;
  for ($i = 0; $i < count((array)$selected_item); $i++) {
    $item[$n_items + $i] = $selected_item[$i];
  }
}

echo "<h2>Select Type of Search</h2>\n";
echo "<form action='./dsbrowse.php' method='get'>\n";

echo "<input name='browsetype' type='radio' value='title'" . ($browsetype == 'title' ? " checked" : "") . ">Title  
      <input name='browse_title' value='" . htmlspecialchars($browse_title) . "' type='text' size='15'><br>\n";

echo "<input name='browsetype' type='radio' value='actor'" . ($browsetype == 'actor' ? " checked" : "") . ">Actor  
      <input name='browse_actor' value='" . htmlspecialchars($browse_actor) . "' type='text' size='15'><br>\n";

echo "<input name='browsetype' type='radio' value='category'" . ($browsetype == 'category' ? " checked" : "") . ">Category\n";

$categories = ["Action", "Animation", "Children", "Classics", "Comedy", "Documentary", "Drama", "Family", "Foreign",
    "Games", "Horror", "Music", "New", "Sci-Fi", "Sports", "Travel"];

echo "<select name='browse_category'>\n";
foreach ($categories as $i => $cat) {
  $value = $i + 1;
  $selected = ($browse_category == $value) ? " selected" : "";
  echo "<option value='$value'$selected>$cat</option>\n";
}
echo "</select><br>\n";

echo "Number of search results to return<br>\n";
echo "<select name='limit_num'>\n";
for ($i = 1; $i <= 10; $i++) {
  $selected = ($limit_num == $i) ? " selected" : "";
  echo "<option value='$i'$selected>$i</option>\n";
}
echo "</select><br>\n";

echo "<input type='hidden' name='customerid' value='" . htmlspecialchars($customerid) . "'>\n";
foreach ((array)$item as $i) {
  echo "<input type='hidden' name='item[]' value='" . htmlspecialchars($i) . "'>\n";
}
echo "<input type='submit' value='Search'>\n";
echo "</form>\n";

// If search requested
if (!empty($browsetype)) {
  $link = new mysqli("dvd2db", "root", "", "dvd2db", 3306);
  if ($link->connect_error) die("DB connection failed: " . $link->connect_error);

  switch ($browsetype) {
    case "title":
      $browse_query = "SELECT * FROM PRODUCTS WHERE MATCH (TITLE) AGAINST (?) LIMIT ?";
      $stmt = $link->prepare($browse_query);
      $stmt->bind_param("si", $browse_title, $limit_num);
      break;
    case "actor":
      $browse_query = "SELECT * FROM PRODUCTS WHERE MATCH (ACTOR) AGAINST (?) LIMIT ?";
      $stmt = $link->prepare($browse_query);
      $stmt->bind_param("si", $browse_actor, $limit_num);
      break;
    case "category":
      $browse_query = "SELECT * FROM PRODUCTS WHERE CATEGORY = ? AND SPECIAL = 1 LIMIT ?";
      $stmt = $link->prepare($browse_query);
      $stmt->bind_param("ii", $browse_category, $limit_num);
      break;
    default:
      $stmt = null;
  }

  if ($stmt && $stmt->execute()) {
    $result = $stmt->get_result();

    if ($result->num_rows === 0) {
      echo "<h2>No DVDs Found</h2>\n";
    } else {
      echo "<br><h2>Search Results</h2>\n";
      echo "<form action='./dsbrowse.php' method='get'>\n";
      echo "<table border=2>\n<tr><th>Add to Shopping Cart</th><th>Title</th><th>Actor</th><th>Price</th></tr>\n";

      while ($row = $result->fetch_assoc()) {
        echo "<tr>\n";
        echo "<td><input name='selected_item[]' type='checkbox' value='{$row['PROD_ID']}'></td>\n";
        echo "<td>{$row['TITLE']}</td>\n";
        echo "<td>{$row['ACTOR']}</td>\n";
        echo "<td>{$row['PRICE']}</td>\n";
        echo "</tr>\n";
      }
      echo "</table><br>\n";

      echo "<input type='hidden' name='customerid' value='" . htmlspecialchars($customerid) . "'>\n";
      foreach ((array)$item as $i) {
        echo "<input type='hidden' name='item[]' value='" . htmlspecialchars($i) . "'>\n";
      }
      echo "<input type='submit' value='Update Shopping Cart'>\n";
      echo "</form>\n";
    }
    $result->free();
    $stmt->close();
  }

  $link->close();
}

// Show Shopping Cart
if (!empty($item)) {
  echo "<h2>Shopping Cart</h2>\n";
  echo "<form action='./dspurchase.php' method='get'>\n";
  echo "<table border=2>\n<tr><th>Item</th><th>Title</th></tr>\n";

  $link = new mysqli("dvd2db", "root", "", "dvd2db", 3306);
  if ($link->connect_error) die("DB connection failed: " . $link->connect_error);

  foreach ((array)$item as $i => $prod_id) {
    $j = $i + 1;
    $query = "SELECT TITLE FROM PRODUCTS WHERE PROD_ID = ?";
    $stmt = $link->prepare($query);
    $stmt->bind_param("i", $prod_id);
    $stmt->execute();
    $stmt->bind_result($title);
    $stmt->fetch();
    echo "<tr><td>$j</td><td>" . htmlspecialchars($title) . "</td></tr>\n";
    $stmt->close();
  }

  echo "</table><br>\n";
  foreach ((array)$item as $i) {
    echo "<input type='hidden' name='item[]' value='" . htmlspecialchars($i) . "'>\n";
  }
  echo "<input type='hidden' name='customerid' value='" . htmlspecialchars($customerid) . "'>\n";
  echo "<input type='submit' value='Checkout'>\n";
  echo "</form>\n";

  $link->close();
}

ds_html_footer();
?>
