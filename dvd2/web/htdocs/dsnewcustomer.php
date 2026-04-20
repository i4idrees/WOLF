<?php
// debug + errors
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

/*
 * DVD Store New Customer PHP Page - dsnewcustomer.php (mysqli)
 */

include("dscommon.inc");
ds_html_header("New Customer Login");

// collect inputs (same as before)
$firstname = isset($_REQUEST["firstname"]) ? $_REQUEST["firstname"] : NULL;
$lastname  = isset($_REQUEST["lastname"]) ? $_REQUEST["lastname"] : NULL;
$address1  = isset($_REQUEST["address1"]) ? $_REQUEST["address1"] : NULL;
$address2  = isset($_REQUEST["address2"]) ? $_REQUEST["address2"] : NULL;
$city      = isset($_REQUEST["city"]) ? $_REQUEST["city"] : NULL;
$state     = isset($_REQUEST["state"]) ? $_REQUEST["state"] : NULL;
$zip       = isset($_REQUEST["zip"]) ? $_REQUEST["zip"] : NULL;
$country   = isset($_REQUEST["country"]) ? $_REQUEST["country"] : NULL;
$email     = isset($_REQUEST["email"]) ? $_REQUEST["email"] : NULL;
$phone     = isset($_REQUEST["phone"]) ? $_REQUEST["phone"] : NULL;
$creditcardtype = isset($_REQUEST["creditcardtype"]) ? $_REQUEST["creditcardtype"] : NULL;
$creditcard  = isset($_REQUEST["creditcard"]) ? $_REQUEST["creditcard"] : NULL;
$ccexpmon  = isset($_REQUEST["ccexpmon"]) ? $_REQUEST["ccexpmon"] : NULL;
$ccexpyr   = isset($_REQUEST["ccexpyr"]) ? $_REQUEST["ccexpyr"] : NULL;
$username  = isset($_REQUEST["username"]) ? $_REQUEST["username"] : NULL;
$password  = isset($_REQUEST["password"]) ? $_REQUEST["password"] : NULL;
$age       = isset($_REQUEST["age"]) ? $_REQUEST["age"] : NULL;
$income    = isset($_REQUEST["income"]) ? $_REQUEST["income"] : NULL;
$gender    = isset($_REQUEST["gender"]) ? $_REQUEST["gender"] : NULL;

// run only if required fields present
if (!( empty($firstname) OR empty($lastname) OR empty($address1) OR empty($city) OR empty($country) 
  OR empty($username) OR empty($password) ))
{
    // connect using mysqli to the docker service 'dvd2db'
    $db_host = "dvd2db";   // container host
    $db_user = "root";
    $db_pass = "";         // empty because MYSQL_ALLOW_EMPTY_PASSWORD=yes
    $db_name = "dvd2db";

    $link = mysqli_connect($db_host, $db_user, $db_pass, $db_name);

    if (!$link) {
        // fatal connection error
        echo "<b>DB connection failed:</b> " . mysqli_connect_error();
        ds_html_footer();
        exit;
    } else {
        echo "<!-- DB connection OK -->\n";
    }

    // --- 1) check username exists (use prepared statement)
    $stmt = mysqli_prepare($link, "SELECT COUNT(*) FROM customers WHERE username = ?");
    mysqli_stmt_bind_param($stmt, "s", $username);
    mysqli_stmt_execute($stmt);
    mysqli_stmt_bind_result($stmt, $count);
    mysqli_stmt_fetch($stmt);
    mysqli_stmt_close($stmt);

    echo "<!-- username exists count = " . intval($count) . " -->\n";

    if ($count != 0) {
        echo "<H2>Username already in use! Please try another username</H2>\n";
        dsnewcustomer_form($firstname,$lastname,$address1,$address2,$city,$state,$zip,$country,
          $email,$phone,$creditcardtype,$creditcard,$ccexpmon,$ccexpyr,$username,$password,$age,$income,$gender);
    } else {
        // compute region & creditcard expiration same logic
        $region = ($country !== "US") ? 2 : 1;
        $creditcardexpiration = sprintf("%4d/%02d", $ccexpyr, $ccexpmon);

        // Build CALL string (we'll escape values to avoid accidental SQL injection)
        // Note: using mysqli_real_escape_string because CALL uses a single concatenated SQL string.
        $fn = mysqli_real_escape_string($link, $firstname);
        $ln = mysqli_real_escape_string($link, $lastname);
        $a1 = mysqli_real_escape_string($link, $address1);
        $a2 = mysqli_real_escape_string($link, $address2);
        $ct = mysqli_real_escape_string($link, $city);
        $st = mysqli_real_escape_string($link, $state);
        $zp = (int)$zip;
        $co = mysqli_real_escape_string($link, $country);
        $rg = (int)$region;
        $em = mysqli_real_escape_string($link, $email);
        $ph = mysqli_real_escape_string($link, $phone);
        $cctype = (int)$creditcardtype;
        $cc = mysqli_real_escape_string($link, $creditcard);
        $ccexp = mysqli_real_escape_string($link, $creditcardexpiration);
        $un = mysqli_real_escape_string($link, $username);
        $pw = mysqli_real_escape_string($link, $password);
        $ag = ($age === NULL || $age === "") ? "NULL" : (int)$age;
        $inc = ($income === NULL || $income === "") ? "NULL" : (int)$income;
        $gn = mysqli_real_escape_string($link, $gender);

        // Build CALL - note we pass numeric values directly and strings quoted
        $call = "CALL NEW_CUSTOMER(" .
            "'$fn','$ln','$a1','$a2','$ct','$st',$zp,'$co'," .
            "$rg,'$em','$ph',$cctype,'$cc','$ccexp'," .
            "'$un','$pw',$ag,$inc,'$gn', @customerid_out)";

        // Debug output so you can see the call when JMX runs
        echo "<!-- CALL: " . htmlspecialchars($call) . " -->\n";

        // Execute the CALL
        if (!mysqli_query($link, $call)) {
            echo "<b>Error executing NEW_CUSTOMER:</b> " . mysqli_error($link) . "<br>\n";
            echo "<!-- Full call: " . htmlspecialchars($call) . " -->\n";
            ds_html_footer();
            mysqli_close($link);
            exit;
        } else {
            echo "<!-- Stored procedure executed OK -->\n";
        }

        // fetch @customerid_out
        $res = mysqli_query($link, "SELECT @customerid_out AS customerid");
        if (!$res) {
            echo "<b>Error fetching @customerid_out:</b> " . mysqli_error($link) . "<br>\n";
            mysqli_close($link);
            ds_html_footer();
            exit;
        }
        $row = mysqli_fetch_assoc($res);
        mysqli_free_result($res);
        $customerid = isset($row['customerid']) ? $row['customerid'] : null;

        if ($customerid === null || $customerid === "") {
            echo "<!-- Procedure executed but returned no customerid (NULL or empty) -->\n";
        } else {
            echo "<!-- New customer id: " . htmlspecialchars($customerid) . " -->\n";
        }

        echo "<H2>New Customer Successfully Added. Click below to begin shopping</H2>\n";
        echo "<FORM ACTION='./dsbrowse.php' METHOD=GET>\n";
        echo "<INPUT TYPE=HIDDEN NAME=customerid VALUE=\"" . htmlspecialchars($customerid) . "\">\n";
        echo "<INPUT TYPE=SUBMIT VALUE='Start Shopping'>\n";
        echo "</FORM>\n";
    }

    mysqli_close($link);
} else {
    echo "<H2>New Customer - Please Complete All Required Fields Below (marked with *)</H2>\n";
    dsnewcustomer_form($firstname,$lastname,$address1,$address2,$city,$state,$zip,$country,
      $email,$phone,$creditcardtype,$creditcard,$ccexpmon,$ccexpyr,$username,$password,$age,$income,$gender);
}

ds_html_footer();


// the form function unchanged from original (keeps same behavior)
function dsnewcustomer_form($firstname,$lastname,$address1,$address2,$city,$state,$zip,$country,
  $email,$phone,$creditcardtype,$creditcard,$ccexpmon,$ccexpyr,$username,$password,$age,$income,$gender)
{
  $countries = array("United States", "Australia", "Canada", "Chile", "China", "France", "Germany", "Japan",
                       "Russia", "South Africa", "UK");

  $cctypes = array("MasterCard", "Visa", "Discover", "Amex", "Dell Preferred");
  $months = array("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec");

  echo "<FORM ACTION='./dsnewcustomer.php' METHOD='GET'>\n";
  echo "Firstname <INPUT TYPE=TEXT NAME='firstname' VALUE='$firstname' SIZE=16 MAXLENGTH=50>* <BR>\n";
  echo "Lastname <INPUT TYPE=TEXT NAME='lastname' VALUE='$lastname' SIZE=16 MAXLENGTH=50>* <BR>\n";
  echo "Address1 <INPUT TYPE=TEXT NAME='address1' VALUE='$address1' SIZE=16 MAXLENGTH=50>* <BR>\n";
  echo "Address2 <INPUT TYPE=TEXT NAME='address2' VALUE='$address2' SIZE=16 MAXLENGTH=50> <BR>\n";
  echo "City <INPUT TYPE=TEXT NAME='city' VALUE='$city' SIZE=16 MAXLENGTH=50>* <BR>\n";
  echo "State <INPUT TYPE=TEXT NAME='state' VALUE='$state' SIZE=16 MAXLENGTH=50> <BR>\n";
  echo "Zipcode <INPUT TYPE=TEXT NAME='zip' VALUE='$zip' SIZE=16 MAXLENGTH='5'> <BR>\n";
  echo "Country <SELECT NAME='country' SIZE=1>\n";
  for ($i=0; $i<count($countries); $i++)
  {
    if ($countries[$i] == $country)
      {echo "  <OPTION VALUE=\"" . htmlspecialchars($countries[$i]) . "\" SELECTED>" . htmlspecialchars($countries[$i]) . "</OPTION>\n";}
    else
      {echo "  <OPTION VALUE=\"" . htmlspecialchars($countries[$i]) . "\">" . htmlspecialchars($countries[$i]) . "</OPTION>\n";}
  }
  echo "</SELECT>* <BR>\n";
  echo "Email <INPUT TYPE=TEXT NAME='email' VALUE='$email' SIZE=16 MAXLENGTH=50> <BR>\n";
  echo "Phone <INPUT TYPE=TEXT NAME='phone' VALUE='$phone' SIZE=16 MAXLENGTH=50> <BR>\n";

  echo "Credit Card Type ";
  echo "<SELECT NAME='creditcardtype' SIZE=1>\n";
  for ($i=0; $i<5; $i++)
  {
    $j = $i + 1;
    if ($j == $creditcardtype)
      {echo "  <OPTION VALUE=\"$j\" SELECTED>" . htmlspecialchars($cctypes[$i]) . "</OPTION>\n";}
    else
      {echo "  <OPTION VALUE=\"$j\">" . htmlspecialchars($cctypes[$i]) . "</OPTION>\n";}
  }
  echo "</SELECT>\n";

  echo "  Credit Card Number <INPUT TYPE=TEXT NAME='creditcard' VALUE='$creditcard' SIZE=16 MAXLENGTH=50>\n";

  echo "  Credit Card Expiration ";
  echo "<SELECT NAME='ccexpmon' SIZE=1>\n";
  for ($i=0; $i<12; $i++)
  {
    $j = $i+1;
    if ($j == $ccexpmon)
      {echo "  <OPTION VALUE=\"$j\" SELECTED>$months[$i]</OPTION>\n";}
    else
      {echo "  <OPTION VALUE=\"$j\">$months[$i]</OPTION>\n";}
  }
  echo "</SELECT>\n";
  echo "<SELECT NAME='ccexpyr' SIZE=1>\n";
  for ($i=0; $i<6; $i++)
  {
    $yr = 2008 + $i;
    if ($yr == $ccexpyr)
      {echo "  <OPTION VALUE=\"$yr\" SELECTED>$yr</OPTION>\n";}
    else
      {echo "  <OPTION VALUE=\"$yr\">$yr</OPTION>\n";}
  }
  echo "</SELECT><BR>\n";

  echo "Username <INPUT TYPE=TEXT NAME='username' VALUE='$username' SIZE=16 MAXLENGTH=50>* <BR>\n";
  echo "Password <INPUT TYPE='PASSWORD' NAME='password' VALUE='$password' SIZE=16 MAXLENGTH=50>* <BR>\n";
  echo "Age <INPUT TYPE=TEXT NAME='age' VALUE='$age' SIZE=3 MAXLENGTH=3> <BR>\n";
  echo "Income (\$US) <INPUT TYPE=TEXT NAME='income' VALUE='$income' SIZE=16 MAXLENGTH=50> <BR>\n";
  echo "Gender <INPUT TYPE=RADIO NAME='gender' VALUE=\"M\" "; if($gender == 'M') echo "CHECKED"; echo "> Male \n";
  echo "       <INPUT TYPE=RADIO NAME='gender' VALUE=\"F\" "; if($gender == 'F') echo "CHECKED"; echo "> Female \n";
  echo "       <INPUT TYPE=RADIO NAME='gender' VALUE=\"?\" "; if($gender == '?' || $gender == '') echo "CHECKED"; echo "> Don't Know <BR>\n";
  echo "<INPUT TYPE='submit' VALUE='Submit New Customer Data'>\n";
  echo "</FORM>\n";
}
?>
