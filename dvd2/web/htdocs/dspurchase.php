<?php
/*  
 * DVD Store Purchase PHP Page - dspurchase.php
 * Updated for PHP 7.4+ compatibility (mysqli)
 */

include("dscommon.inc");

ds_html_header("DVD Store Purchase Page");

$confirmpurchase = $_REQUEST["confirmpurchase"] ?? null;
$item = $_REQUEST["item"] ?? [];
$quan = $_REQUEST["quan"] ?? [];
$drop = $_REQUEST["drop"] ?? [];
$customerid = $_REQUEST["customerid"] ?? null;
$netamount = 0;

$link = new mysqli("dvd2db", "root", "", "dvd2db", 3306);
//$link = new mysqli("localhost", "root", "", "dvd2db", 3310);
if ($link->connect_error) die("Connection failed: " . $link->connect_error);

if (empty($confirmpurchase)) {
  echo "<h2>Selected Items: specify quantity desired; click Purchase when finished</h2><br>";
  echo "<form action='./dspurchase.php' method='get'>";
  echo "<table border=2><tr><th>Item</th><th>Quantity</th><th>Title</th><th>Actor</th><th>Price</th><th>Remove?</th></tr>";

  $j = 0;
  foreach ($item as $i => $prod_id) {
    if (empty($drop) || !in_array($i, $drop)) {
      $stmt = $link->prepare("SELECT PROD_ID, TITLE, ACTOR, PRICE FROM PRODUCTS WHERE PROD_ID = ?");
      $stmt->bind_param("i", $prod_id);
      $stmt->execute();
      $stmt->bind_result($pid, $title, $actor, $price);
      if ($stmt->fetch()) {
        ++$j;
        $qty = max(1, $quan[$i] ?? 1);
        $netamount += $qty * $price;
        echo "<tr>
                        <td align='center'>$j</td>
                        <input type='hidden' name='item[]' value='$pid'>
                        <td><input name='quan[]' type='text' size=10 value='$qty'></td>
                        <td>$title</td>
                        <td>$actor</td>
                        <td align='right'>" . sprintf("$%.2f", $price) . "</td>
                        <td align='center'><input name='drop[]' type='checkbox' value='$i'></td>
                      </tr>";
      }
      $stmt->close();
    }
  }

  $taxpct = 8.25;
  $taxamount = $netamount * $taxpct / 100.0;
  $totalamount = $taxamount + $netamount;
  echo "<tr><td colspan=4>Subtotal</td><td align='right'>$" . sprintf("%.2f", $netamount) . "</td></tr>";
  echo "<tr><td colspan=4>Tax ($taxpct%)</td><td align='right'>$" . sprintf("%.2f", $taxamount) . "</td></tr>";
  echo "<tr><td colspan=4>Total</td><td align='right'>$" . sprintf("%.2f", $totalamount) . "</td></tr>";
  echo "</table><br>";

  echo "<input type='hidden' name='customerid' value='$customerid'>";
  echo "<input type='submit' value='Update and Recalculate Total'>";
  echo "</form><br>";

  echo "<form action='./dspurchase.php' method='get'>
          <input type='hidden' name='confirmpurchase' value='yes'>
          <input type='hidden' name='customerid' value='$customerid'>";
  foreach ($item as $i => $prod_id) {
    if (empty($drop) || !in_array($i, $drop)) {
      $qty = $quan[$i] ?? 1;
      echo "<input name='item[]' type='hidden' value='$prod_id'>
                  <input name='quan[]' type='hidden' value='$qty'>";
    }
  }
  echo "<input type='submit' value='Purchase'>
          </form>";
} else {
  $link->begin_transaction();

  echo "<h2>Purchase complete</h2><table border=2><tr><th>Item</th><th>Quantity</th><th>Title</th><th>Actor</th><th>Price</th></tr>";
  $success = true;
  $orderlines = [];
  $cust_hist = [];

  foreach ($item as $i => $prod_id) {
    $qty = max(1, $quan[$i] ?? 1);
    $stmt = $link->prepare("SELECT TITLE, ACTOR, PRICE, QUAN_IN_STOCK, SALES FROM PRODUCTS JOIN INVENTORY USING (PROD_ID) WHERE PROD_ID = ?");
    $stmt->bind_param("i", $prod_id);
    $stmt->execute();
    $stmt->bind_result($title, $actor, $price, $stock, $sales);
    if ($stmt->fetch()) {
      if ($stock < $qty) {
        echo "<tr><td colspan=5>Insufficient stock for $title</td></tr>";
        $success = false;
      } else {
        $new_stock = $stock - $qty;
        $new_sales = $sales + $qty;
        $link->query("UPDATE INVENTORY SET QUAN_IN_STOCK = $new_stock, SALES = $new_sales WHERE PROD_ID = $prod_id");
        $netamount += $qty * $price;
        $orderlines[] = "(NULL, ?, $prod_id, $qty, CURDATE())";
        $cust_hist[] = "($customerid, ?, $prod_id)";
        $j = $i + 1;
        echo "<tr><td>$j</td><td>$qty</td><td>$title</td><td>$actor</td><td>" . sprintf("$%.2f", $price) . "</td></tr>";
      }
    }
    $stmt->close();
  }

  $taxamount = $netamount * 8.25 / 100.0;
  $totalamount = $taxamount + $netamount;
  echo "<tr><td colspan=4>Subtotal</td><td align='right'>$" . sprintf("%.2f", $netamount) . "</td></tr>";
  echo "<tr><td colspan=4>Tax (8.25%)</td><td align='right'>$" . sprintf("%.2f", $taxamount) . "</td></tr>";
  echo "<tr><td colspan=4>Total</td><td align='right'>$" . sprintf("%.2f", $totalamount) . "</td></tr>";
  echo "</table><br>";

  if ($success) {
    $link->query("INSERT INTO ORDERS (ORDERDATE, CUSTOMERID, NETAMOUNT, TAX, TOTALAMOUNT) VALUES (CURDATE(), $customerid, $netamount, $taxamount, $totalamount)");
    $orderid = $link->insert_id;

    if ($orderid > 0) {
      foreach ($orderlines as &$line) {
        $line = str_replace("?", $orderid, $line);
      }
      $link->query("INSERT INTO ORDERLINES (ORDERLINEID, ORDERID, PROD_ID, QUANTITY, ORDERDATE) VALUES " . implode(",", $orderlines));

      foreach ($cust_hist as &$hist) {
        $hist = str_replace("?", $orderid, $hist);
      }
      $link->commit();
      $link->query("INSERT INTO CUST_HIST (CUSTOMERID, ORDERID, PROD_ID) VALUES " . implode(",", $cust_hist));

      $cc_result = $link->query("SELECT CREDITCARDTYPE, CREDITCARD, CREDITCARDEXPIRATION FROM customers WHERE CUSTOMERID = $customerid");
      $cc_row = $cc_result->fetch_row();
      $cctypes = ["MasterCard", "Visa", "Discover", "Amex", "Dell Preferred"];
      echo "<h3>$" . sprintf("%.2f", $totalamount) . " charged to credit card $cc_row[1] (" . $cctypes[$cc_row[0] - 1] . "), expiration $cc_row[2]</h3><br>";
      echo "<h2>Order Completed Successfully --- ORDER NUMBER: $orderid</h2><br>";
    } else {
      $link->rollback();
      echo "<h3>Order failed: Could not insert order record.</h3>";
    }
  } else {
    $link->rollback();
    echo "<h3>Insufficient stock - order not processed</h3>";
  }
}

$link->close();
ds_html_footer();
?>
