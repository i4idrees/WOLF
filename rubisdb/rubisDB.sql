-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Oct 31, 2024 at 01:39 AM
-- Server version: 10.1.8-MariaDB
-- PHP Version: 5.6.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `rubis`
--

-- --------------------------------------------------------

--
-- Table structure for table `bids`
--

CREATE TABLE `bids` (
  `id` int(10) UNSIGNED NOT NULL,
  `user_id` int(10) UNSIGNED NOT NULL,
  `item_id` int(10) UNSIGNED NOT NULL,
  `qty` int(10) UNSIGNED NOT NULL,
  `bid` float UNSIGNED NOT NULL,
  `max_bid` float UNSIGNED NOT NULL,
  `date` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `bids`
--

INSERT INTO `bids` (`id`, `user_id`, `item_id`, `qty`, `bid`, `max_bid`, `date`) VALUES
(37, 32, 10490, 5, 1001, 1005, '2024-10-30 13:39:30'),
(38, 32, 10490, 5, 1001, 1005, '2024-10-30 13:39:31'),
(39, 32, 10490, 5, 1001, 1005, '2024-10-30 13:39:32'),
(40, 32, 10490, 5, 1001, 1005, '2024-10-30 13:39:32');

-- --------------------------------------------------------

--
-- Table structure for table `buy_now`
--

CREATE TABLE `buy_now` (
  `id` int(10) UNSIGNED NOT NULL,
  `buyer_id` int(10) UNSIGNED NOT NULL,
  `item_id` int(10) UNSIGNED NOT NULL,
  `qty` int(10) UNSIGNED NOT NULL,
  `date` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `categories`
--

CREATE TABLE `categories` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `categories`
--

INSERT INTO `categories` (`id`, `name`) VALUES
(1, 'Antiques & Art '),
(2, 'Books '),
(3, 'Business, Office & Industrial '),
(4, 'Clothing & Accessories '),
(5, 'Coins '),
(6, 'Collectibles '),
(7, 'Computers '),
(8, 'Consumer Electronics '),
(9, 'Dolls & Bears '),
(10, 'Home & Garden '),
(11, 'Jewelry, Gems & Watches '),
(12, 'Movies & Television '),
(13, 'Music '),
(14, 'Photo '),
(15, 'Pottery & Glass '),
(16, 'Sports '),
(17, 'Stamps '),
(18, 'Tickets & Travel '),
(19, 'Toys & Hobbies '),
(20, 'Everything Else '),
(375, 'Antiques & Art'),
(376, 'General'),
(377, 'Antique Prints'),
(378, 'Antiquities'),
(379, 'Architectural'),
(380, 'Art: Fine'),
(381, 'Asian Antiques'),
(382, 'Books, Manuscripts'),
(383, 'Ceramics'),
(384, 'Ethnographic'),
(385, 'European'),
(386, 'Folk Art'),
(387, 'Furniture'),
(388, 'Glass'),
(389, 'Maps, Atlases'),
(390, 'Maritime'),
(391, 'Medical'),
(392, 'Metalware'),
(393, 'Musical Instruments'),
(394, 'Primitives'),
(395, 'Reproductions'),
(396, 'Science Instruments'),
(397, 'Silver'),
(398, 'Silver Plate'),
(399, 'Textiles, Linens'),
(400, 'Toleware'),
(401, 'Woodenware'),
(402, 'Antiques'),
(403, 'Books'),
(404, 'Antiquarian, Rare'),
(405, 'Audio'),
(406, 'Braille'),
(407, 'Children'),
(408, 'Cookbooks'),
(409, 'Fiction'),
(410, 'First Editions'),
(411, 'Large Print'),
(412, 'Magazines'),
(413, 'Nonfiction'),
(414, 'Poetry'),
(415, 'Pulps, Club Editions'),
(416, 'Reference'),
(417, 'School Annuals'),
(418, 'Textbooks, Education'),
(419, 'Vintage Paperbacks'),
(420, 'Other'),
(421, 'Business, Office & Industrial'),
(422, 'Agriculture'),
(423, 'Aviation'),
(424, 'Businesses for Sale'),
(425, 'Construction'),
(426, 'Electronic Parts'),
(427, 'General Equipment'),
(428, 'Laboratory Equipment'),
(429, 'Medical, Dental'),
(430, 'Metalworking Equipment'),
(431, 'Office Equipment'),
(432, 'Office Furniture'),
(433, 'Office Supplies'),
(434, 'Printing Equipment'),
(435, 'Restaurant'),
(436, 'Retail'),
(437, 'Safety and Security'),
(438, 'Telephone Systems'),
(439, 'Test Equipment'),
(440, 'Wholesale, Large Lots'),
(441, 'Clothing & Accessories'),
(442, 'Infants'),
(443, 'Boys'),
(444, 'Girls'),
(445, 'Men'),
(446, 'Personal Care'),
(447, 'Wedding Apparel'),
(448, 'Women'),
(449, 'Coins'),
(450, 'Coins: US'),
(451, 'Coins: World'),
(452, 'Exonumia'),
(453, 'Paper Money: US'),
(454, 'Paper Money: World'),
(455, 'Scripophily'),
(456, 'Collectibles'),
(457, 'Advertising'),
(458, 'Animals'),
(459, 'Art, Animation & Photo Images'),
(460, 'Autographs, Paper & Writing'),
(461, 'Coin-Op, Banks & Casino'),
(462, 'Cultures & Religions'),
(463, 'Decorative & Holiday'),
(464, 'Disneyana'),
(465, 'Furnishings, Knives & Tools'),
(466, 'Historical Memorabilia'),
(467, 'Housewares & Kitchenware'),
(468, 'Militaria'),
(469, 'Pop Culture'),
(470, 'Science, Medical'),
(471, 'Transportation'),
(472, 'Vintage Clothing, Accessories'),
(473, 'Computers'),
(474, 'Apple, Macintosh'),
(475, 'Desktop Components'),
(476, 'Desktop PCs'),
(477, 'Domain Names'),
(478, 'Drives, Media'),
(479, 'Input Peripherals'),
(480, 'Laptops & Accessories'),
(481, 'Monitors'),
(482, 'Networking'),
(483, 'Other Hardware'),
(484, 'Printers & Supplies'),
(485, 'Scanners'),
(486, 'Services'),
(487, 'Software'),
(488, 'Technology Books'),
(489, 'Video & Multimedia'),
(490, 'Consumer Electronics'),
(491, 'Car Audio & Electronics'),
(492, 'Gadgets & Other Electronics'),
(493, 'Home Audio & Video'),
(494, 'PDAs/Handheld PCs'),
(495, 'Phones & Wireless Devices'),
(496, 'Portable Audio & Video'),
(497, 'Radio Equipment'),
(498, 'Video Games'),
(499, 'Dolls & Bears'),
(500, 'Barbie'),
(501, 'Bears'),
(502, 'Doll Clothes, Furniture'),
(503, 'Doll Making, Patterns, Repair'),
(504, 'Dolls'),
(505, 'Houses, Miniatures'),
(506, 'Paper Dolls'),
(507, 'Home & Garden'),
(508, 'Baby Items'),
(509, 'Building Supplies'),
(510, 'Food & Beverage'),
(511, 'Garden Items'),
(512, 'Home Furnishings'),
(513, 'Household Items'),
(514, 'Pet Supplies'),
(515, 'Solar Energy'),
(516, 'Swimming Pools'),
(517, 'Tools'),
(518, 'Jewelry, Gems & Watches'),
(519, 'Beads, Amulets'),
(520, 'Costume Jewelry'),
(521, 'Ethnic, Tribal Jewelry'),
(522, 'Fine Jewelry'),
(523, 'Hair Jewelry'),
(524, 'Designer, Artisan Jewelry'),
(525, 'Jewelry Boxes'),
(526, 'Jewelry Supplies'),
(527, 'Loose Gemstones'),
(528, 'Men'),
(529, 'Watches'),
(530, 'Movies & Television'),
(531, 'Memorabilia'),
(532, 'Video, Film'),
(533, 'Music'),
(534, 'Music Memorabilia'),
(535, 'Musical Instruments'),
(536, 'Printed, Recorded'),
(537, 'Photo'),
(538, 'Camera Accessories'),
(539, 'Darkroom Equip, Supplies'),
(540, 'Digital Cameras'),
(541, 'Film'),
(542, 'Film Cameras'),
(543, 'Lenses'),
(544, 'Projection Equipment'),
(545, 'Vintage'),
(546, 'Pottery & Glass'),
(547, 'Glass'),
(548, 'Pottery & China'),
(549, 'Sports'),
(550, 'Autographs'),
(551, 'Memorabilia'),
(552, 'Sporting Goods'),
(553, 'Trading Cards'),
(554, 'Stamps'),
(555, 'United States'),
(556, 'Australia'),
(557, 'Canada'),
(558, 'Br. Comm. Other'),
(559, 'UK (Great Britain)'),
(560, 'Europe'),
(561, 'Latin America'),
(562, 'Other World'),
(563, 'Philately'),
(564, 'Topical'),
(565, 'Tickets & Travel'),
(566, 'Tickets & Experiences'),
(567, 'Travel'),
(568, 'Toys & Hobbies'),
(569, 'Action Figures'),
(570, 'Building Toys'),
(571, 'Classic Toys'),
(572, 'Diecast, Toy Vehicles'),
(573, 'Educational, Developmental'),
(574, 'Electronic, Battery, Wind-Up'),
(575, 'Fast Food, Advertising'),
(576, 'Games'),
(577, 'Hobbies & Crafts'),
(578, 'Outdoor Toys, Structures'),
(579, 'Pretend Play, Make-Believe'),
(580, 'Puzzles'),
(581, 'Robots, Monsters, Space Toys'),
(582, 'Stuffed Animals, Beanbag'),
(583, 'Toy Soldiers'),
(584, 'TV, Character Toys'),
(585, 'Vintage, Antique Toys'),
(586, 'Everything Else'),
(587, 'Body Art, Tattoos'),
(588, 'Candles, Candle Holders'),
(589, 'Costumes'),
(590, 'eBay Stuff'),
(591, 'Education & Learning'),
(592, 'Genealogy'),
(593, 'Gifts & Party Supplies'),
(594, 'Health & Wellness'),
(595, 'Memberships'),
(596, 'Metaphysical'),
(597, 'Novelties: Risque'),
(598, 'Optics'),
(599, 'Personal Security'),
(600, 'Religious Products & Supplies'),
(601, 'Reward Points, Incentive Progs'),
(602, 'Services'),
(603, 'Shipping & Packing Supplies'),
(604, 'Show Supplies'),
(605, 'Subscriptions'),
(606, 'Test Auctions'),
(607, 'Uniforms'),
(608, 'Vehicles (AFA)'),
(609, 'Vehicle Parts, Access. (AFA)'),
(610, 'Weird Stuff');

-- --------------------------------------------------------

--
-- Table structure for table `comments`
--

CREATE TABLE `comments` (
  `id` int(10) UNSIGNED NOT NULL,
  `from_user_id` int(10) UNSIGNED NOT NULL,
  `to_user_id` int(10) UNSIGNED NOT NULL,
  `item_id` int(10) UNSIGNED NOT NULL,
  `rating` int(11) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `comment` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `comments`
--

INSERT INTO `comments` (`id`, `from_user_id`, `to_user_id`, `item_id`, `rating`, `date`, `comment`) VALUES
(36, 32, 32, 10490, 3, '2024-10-30 13:39:30', 'Write your comment here'),
(37, 32, 32, 10490, 3, '2024-10-30 13:39:31', 'Write your comment here'),
(38, 32, 32, 10490, 3, '2024-10-30 13:39:32', 'Write your comment here'),
(39, 32, 32, 10490, 3, '2024-10-30 13:39:32', 'Write your comment here');

-- --------------------------------------------------------

--
-- Table structure for table `ids`
--

CREATE TABLE `ids` (
  `id` int(10) UNSIGNED NOT NULL,
  `category` int(10) UNSIGNED NOT NULL,
  `region` int(10) UNSIGNED NOT NULL,
  `users` int(10) UNSIGNED NOT NULL,
  `item` int(10) UNSIGNED NOT NULL,
  `comment` int(10) UNSIGNED NOT NULL,
  `bid` int(10) UNSIGNED NOT NULL,
  `buyNow` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `items`
--

CREATE TABLE `items` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `description` text,
  `initial_price` float UNSIGNED NOT NULL,
  `quantity` int(10) UNSIGNED NOT NULL,
  `reserve_price` float UNSIGNED DEFAULT '0',
  `buy_now` float UNSIGNED DEFAULT '0',
  `nb_of_bids` int(10) UNSIGNED DEFAULT '0',
  `max_bid` float UNSIGNED DEFAULT '0',
  `start_date` datetime DEFAULT NULL,
  `end_date` datetime DEFAULT NULL,
  `seller` int(10) UNSIGNED NOT NULL,
  `category` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `items`
--

INSERT INTO `items` (`id`, `name`, `description`, `initial_price`, `quantity`, `reserve_price`, `buy_now`, `nb_of_bids`, `max_bid`, `start_date`, `end_date`, `seller`, `category`) VALUES
(10490, 'item1', 'item1', 1000, 5, 1000, 1000, 471064, 0, '2024-09-24 21:27:01', '2024-10-01 21:27:01', 32, 1),
(10527, 'item1', 'item1', 1000, 5, 1000, 1000, 0, 0, '2024-10-30 13:39:30', '2024-11-06 13:39:30', 32, 1),
(10528, 'item1', 'item1', 1000, 5, 1000, 1000, 0, 0, '2024-10-30 13:39:31', '2024-11-06 13:39:31', 32, 1),
(10529, 'item1', 'item1', 1000, 5, 1000, 1000, 0, 0, '2024-10-30 13:39:32', '2024-11-06 13:39:32', 32, 1),
(10530, 'item1', 'item1', 1000, 5, 1000, 1000, 0, 0, '2024-10-30 13:39:32', '2024-11-06 13:39:32', 32, 1);

-- --------------------------------------------------------

--
-- Table structure for table `old_items`
--

CREATE TABLE `old_items` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `description` text,
  `initial_price` float UNSIGNED NOT NULL,
  `quantity` int(10) UNSIGNED NOT NULL,
  `reserve_price` float UNSIGNED DEFAULT '0',
  `buy_now` float UNSIGNED DEFAULT '0',
  `nb_of_bids` int(10) UNSIGNED DEFAULT '0',
  `max_bid` float UNSIGNED DEFAULT '0',
  `start_date` datetime DEFAULT NULL,
  `end_date` datetime DEFAULT NULL,
  `seller` int(10) UNSIGNED NOT NULL,
  `category` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `regions`
--

CREATE TABLE `regions` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(25) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `regions`
--

INSERT INTO `regions` (`id`, `name`) VALUES
(1, 'AZ--Phoenix'),
(2, 'CA--Los Angeles'),
(3, 'CA--Oakland'),
(4, 'CA--Sacramento'),
(5, 'CA--San Diego'),
(6, 'CA--San Francisco'),
(7, 'CA--San Jose'),
(8, 'CO--Denver'),
(9, 'CT--Hartford'),
(10, 'DC--Washington'),
(11, 'FL--Jacksonville'),
(12, 'FL--Miami'),
(13, 'FL--Orlando'),
(14, 'FL--Tampa-St. Pete'),
(15, 'FL--West Palm Beach'),
(16, 'GA--Atlanta'),
(17, 'HI--Honolulu'),
(18, 'ID--Billings-Boise'),
(19, 'IL--Chicago'),
(20, 'IN--Indianapolis'),
(21, 'KS--Kansas City'),
(22, 'KY--Louisville'),
(23, 'LA--New Orleans'),
(24, 'MA--Boston '),
(25, 'MD--Baltimore'),
(26, 'MI--Detroit'),
(27, 'MI--Grand Rapids'),
(28, 'MN--Minn-St. Paul'),
(29, 'MO--Kansas City'),
(30, 'MO--St. Louis'),
(31, 'MT--Billings-Boise'),
(32, 'NC--Charlotte'),
(33, 'NC--Greensboro'),
(34, 'NC--Raleigh-Durham'),
(35, 'ND--Bismarck-Pierre'),
(36, 'NM--Albuquerque'),
(37, 'NV--Las Vegas'),
(38, 'NY--Albany'),
(39, 'NY--Buffalo'),
(40, 'NY--New York'),
(41, 'NY--Rochester'),
(42, 'OH--Cincinnati'),
(43, 'OH--Cleveland'),
(44, 'OH--Columbus'),
(45, 'OH--Dayton'),
(46, 'OK--Oklahoma City'),
(47, 'OR--Portland'),
(48, 'PA--Philadelphia'),
(49, 'PA--Pittsburgh'),
(50, 'RI--Providence'),
(51, 'SD--Bismarck-Pierre'),
(52, 'TN--Memphis'),
(53, 'TN--Nashville'),
(54, 'TX--Austin'),
(55, 'TX--Dallas-Fort Worth'),
(56, 'TX--Houston'),
(57, 'TX--San Antonio'),
(58, 'UT--Salt Lake City'),
(59, 'VA--Norfolk-VA Beach'),
(60, 'VA--Richmond'),
(61, 'WA--Seattle-Tacoma'),
(62, 'WI--Milwaukee');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` int(10) UNSIGNED NOT NULL,
  `firstname` varchar(20) DEFAULT NULL,
  `lastname` varchar(20) DEFAULT NULL,
  `nickname` varchar(20) NOT NULL,
  `password` varchar(20) NOT NULL,
  `email` varchar(50) NOT NULL,
  `rating` int(11) DEFAULT NULL,
  `balance` float DEFAULT NULL,
  `creation_date` datetime DEFAULT NULL,
  `region` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `firstname`, `lastname`, `nickname`, `password`, `email`, `rating`, `balance`, `creation_date`, `region`) VALUES
(32, 'idrees', 'ahmad', 'test1', 'admin', 'admin', 1173774, 0, '2024-07-31 14:41:30', 1),
(64, 'Jane', 'Doe', 'user_uvxxjr', 'axaemitqvd', 'user_uvxxjr@example.com', 0, 0, '2024-10-30 13:39:29', 1),
(65, 'Charlie', 'Brown', 'user_ysewmh', 'kxrbifhlhd', 'user_ysewmh@test.com', 0, 0, '2024-10-30 13:39:31', 1),
(66, 'Jane', 'Smith', 'user_qxskvz', 'kyxwqchqts', 'user_qxskvz@demo.com', 0, 0, '2024-10-30 13:39:32', 1);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `bids`
--
ALTER TABLE `bids`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_bids_item` (`item_id`),
  ADD KEY `idx_bids_user` (`user_id`);

--
-- Indexes for table `buy_now`
--
ALTER TABLE `buy_now`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_buy_now_buyer` (`buyer_id`),
  ADD KEY `idx_buy_now_item` (`item_id`);

--
-- Indexes for table `categories`
--
ALTER TABLE `categories`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `comments`
--
ALTER TABLE `comments`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_comments_from_user` (`from_user_id`),
  ADD KEY `idx_comments_to_user` (`to_user_id`),
  ADD KEY `idx_comments_item` (`item_id`);

--
-- Indexes for table `ids`
--
ALTER TABLE `ids`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `items`
--
ALTER TABLE `items`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_items_seller` (`seller`),
  ADD KEY `idx_items_category` (`category`);

--
-- Indexes for table `old_items`
--
ALTER TABLE `old_items`
  ADD PRIMARY KEY (`id`),
  ADD KEY `idx_old_items_seller` (`seller`),
  ADD KEY `idx_old_items_category` (`category`);

--
-- Indexes for table `regions`
--
ALTER TABLE `regions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `nickname_2` (`nickname`),
  ADD UNIQUE KEY `nickname_4` (`nickname`),
  ADD UNIQUE KEY `nickname` (`nickname`),
  ADD KEY `idx_users_auth` (`nickname`,`password`),
  ADD KEY `idx_users_region` (`region`),
  ADD KEY `nickname_3` (`nickname`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `bids`
--
ALTER TABLE `bids`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=41;
--
-- AUTO_INCREMENT for table `buy_now`
--
ALTER TABLE `buy_now`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `categories`
--
ALTER TABLE `categories`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=611;
--
-- AUTO_INCREMENT for table `comments`
--
ALTER TABLE `comments`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=40;
--
-- AUTO_INCREMENT for table `ids`
--
ALTER TABLE `ids`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `items`
--
ALTER TABLE `items`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10531;
--
-- AUTO_INCREMENT for table `regions`
--
ALTER TABLE `regions`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=63;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=67;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `bids`
--
ALTER TABLE `bids`
  ADD CONSTRAINT `fk_bids_user` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`);

--
-- Constraints for table `buy_now`
--
ALTER TABLE `buy_now`
  ADD CONSTRAINT `fk_buy_now_buyer` FOREIGN KEY (`buyer_id`) REFERENCES `users` (`id`);

--
-- Constraints for table `comments`
--
ALTER TABLE `comments`
  ADD CONSTRAINT `fk_comments_from_user` FOREIGN KEY (`from_user_id`) REFERENCES `users` (`id`),
  ADD CONSTRAINT `fk_comments_to_user` FOREIGN KEY (`to_user_id`) REFERENCES `users` (`id`);

--
-- Constraints for table `items`
--
ALTER TABLE `items`
  ADD CONSTRAINT `fk_items_category` FOREIGN KEY (`category`) REFERENCES `categories` (`id`),
  ADD CONSTRAINT `fk_items_seller` FOREIGN KEY (`seller`) REFERENCES `users` (`id`);

--
-- Constraints for table `old_items`
--
ALTER TABLE `old_items`
  ADD CONSTRAINT `fk_old_items_category` FOREIGN KEY (`category`) REFERENCES `categories` (`id`),
  ADD CONSTRAINT `fk_old_items_seller` FOREIGN KEY (`seller`) REFERENCES `users` (`id`);

--
-- Constraints for table `users`
--
ALTER TABLE `users`
  ADD CONSTRAINT `fk_users_region` FOREIGN KEY (`region`) REFERENCES `regions` (`id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
