-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jan 22, 2025 at 04:20 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `bookstore`
--

-- --------------------------------------------------------

--
-- Table structure for table `book`
--

CREATE TABLE `book` (
  `BookID` int(11) NOT NULL,
  `ISBN` varchar(15) NOT NULL,
  `Title` varchar(100) NOT NULL,
  `Price` decimal(10,2) NOT NULL,
  `Stock` int(11) NOT NULL,
  `Author` varchar(100) NOT NULL,
  `Publisher` varchar(50) NOT NULL,
  `PublishedYear` varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `book`
--

INSERT INTO `book` (`BookID`, `ISBN`, `Title`, `Price`, `Stock`, `Author`, `Publisher`, `PublishedYear`) VALUES
(1, '9780141439600', 'Kamehameha', 21.00, 35, 'Jane Austen', 'Ahirht Publishing', '1813'),
(2, '9780140449136', 'Reincarnated As A Kangroo', 0.50, 25, 'Homer', 'Penguin Classics', '800'),
(3, '9781501124020', 'The Great Gatsby', 25.00, 19, 'F. Scott Fitzgerald', 'Scribner', '1925'),
(4, '9780439139595', 'Freak The Mighty', 77.90, 45, 'Rodman Philbrick', 'Scholastic', '2000'),
(6, '9780061120084', 'To Kill a Mockingbird', 100.00, 12, 'Harper Lee', 'J.B. Lippincott', '1960'),
(7, '9780451524935', '1984', 90.10, 32, 'George Orwell', 'Signet Classics', '1949'),
(8, '9780743273565', 'The Catcher in the Rye', 78.00, 59, 'J.D. Salinger', 'Little, Brown and Company', '1951'),
(9, '9780307277671', 'The Road', 89.00, 35, 'Cormac McCarthy', 'Vintage Books', '2006'),
(10, '9871203984756', 'The Rides to Lake Silberneherze', 20.20, 43, 'Enya Silverweiss', 'Kjerag Publishing Ltd', '2009'),
(11, '9781234567897', 'Whispers of the Forest', 45.50, 46, 'Eleanor Greenwood', 'Emerald Press', '2023'),
(12, '9789876543210', 'Echoes of Eternity', 52.75, 18, 'Marcus Stone', 'Silver Horizon', '2021'),
(13, '9782345678901', 'The Shadow’s Keeper', 60.99, 13, 'Clara Nightshade', 'Obsidian Press', '2022'),
(14, '9783456789012', 'Winds of Solace', 38.40, 24, 'Noah Ellison', 'Golden Feather', '2020');

-- --------------------------------------------------------

--
-- Table structure for table `book_order`
--

CREATE TABLE `book_order` (
  `orderID` int(11) NOT NULL,
  `BookID` int(11) NOT NULL,
  `quantity` int(11) DEFAULT 1,
  `price` decimal(10,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `book_order`
--

INSERT INTO `book_order` (`orderID`, `BookID`, `quantity`, `price`) VALUES
(1, 1, 2, 42.00),
(1, 3, 1, 25.00),
(1, 6, 1, 100.00),
(2, 10, 1, 20.20),
(2, 11, 2, 91.00),
(3, 9, 1, 89.00),
(4, 4, 1, 77.90),
(5, 7, 1, 90.10),
(6, 8, 1, 78.00),
(7, 12, 1, 52.75),
(8, 13, 1, 60.99),
(9, 14, 1, 38.40),
(10, 1, 1, 21.00),
(11, 2, 1, 0.50),
(12, 3, 1, 25.00),
(13, 6, 1, 100.00),
(14, 7, 1, 90.10),
(15, 8, 1, 78.00),
(16, 9, 1, 89.00),
(17, 11, 1, 45.50),
(18, 12, 1, 52.75),
(19, 13, 1, 60.99),
(20, 14, 1, 38.40),
(21, 1, 1, 21.00),
(22, 2, 1, 0.50),
(23, 3, 1, 25.00),
(24, 6, 1, 100.00),
(25, 7, 1, 90.10),
(26, 8, 1, 78.00),
(27, 9, 1, 89.00),
(28, 10, 1, 20.20),
(29, 11, 1, 45.50),
(30, 12, 1, 52.75),
(31, 1, 4, 84.00),
(31, 2, 1, 0.50),
(31, 3, 2, 50.00),
(31, 10, 1, 20.20),
(32, 1, 1, 21.00),
(32, 2, 3, 1.50),
(32, 10, 1, 20.20),
(32, 11, 3, 136.50);

-- --------------------------------------------------------

--
-- Table structure for table `order`
--

CREATE TABLE `order` (
  `orderID` int(11) NOT NULL,
  `orderDate` date NOT NULL,
  `orderStatus` varchar(15) NOT NULL,
  `totalAmount` decimal(10,2) NOT NULL,
  `UserID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `order`
--

INSERT INTO `order` (`orderID`, `orderDate`, `orderStatus`, `totalAmount`, `UserID`) VALUES
(1, '2024-01-01', 'completed', 145.50, 2),
(2, '2024-01-15', 'completed', 120.20, 3),
(3, '2024-01-20', 'completed', 89.00, 4),
(4, '2024-02-05', 'completed', 77.90, 5),
(5, '2024-02-14', 'completed', 90.10, 2),
(6, '2024-03-01', 'completed', 78.00, 3),
(7, '2024-03-10', 'completed', 52.75, 4),
(8, '2024-04-05', 'completed', 60.99, 5),
(9, '2024-04-20', 'completed', 38.40, 2),
(10, '2024-05-01', 'completed', 21.00, 3),
(11, '2024-05-15', 'completed', 0.50, 4),
(12, '2024-06-01', 'completed', 25.00, 5),
(13, '2024-06-10', 'completed', 100.00, 2),
(14, '2024-07-01', 'completed', 90.10, 3),
(15, '2024-07-15', 'completed', 78.00, 4),
(16, '2024-08-01', 'completed', 89.00, 5),
(17, '2024-09-05', 'Completed', 45.50, 2),
(18, '2024-09-15', 'Completed', 52.75, 3),
(19, '2024-10-01', 'Completed', 60.99, 4),
(20, '2024-10-10', 'Completed', 38.40, 5),
(21, '2024-11-01', 'Completed', 21.00, 2),
(22, '2024-11-15', 'Completed', 0.50, 3),
(23, '2024-12-01', 'Completed', 25.00, 4),
(24, '2024-12-10', 'Completed', 100.00, 5),
(25, '2024-12-20', 'Completed', 90.10, 2),
(26, '2025-01-05', 'Completed', 78.00, 3),
(27, '2025-01-10', 'Completed', 89.00, 4),
(28, '2025-01-15', 'Completed', 20.20, 5),
(29, '2025-01-20', 'Completed', 45.50, 2),
(30, '2025-01-25', 'Completed', 52.75, 3),
(31, '2025-01-21', 'completed', 154.70, 10),
(32, '2025-01-22', 'completed', 179.20, 11);

-- --------------------------------------------------------

--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `UserID` int(11) NOT NULL,
  `Name` varchar(100) NOT NULL,
  `IC_no` varchar(25) NOT NULL,
  `Phone_no` varchar(25) NOT NULL,
  `Address` varchar(100) NOT NULL,
  `username` varchar(25) NOT NULL,
  `password` varchar(100) NOT NULL,
  `Role` varchar(25) NOT NULL DEFAULT 'customer'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `user`
--

INSERT INTO `user` (`UserID`, `Name`, `IC_no`, `Phone_no`, `Address`, `username`, `password`, `Role`) VALUES
(1, 'Engku Amir', '040525110517', '0137529380', 'Lot 7428,Wakaf Che Husin,Jalan Intan,23000,Dungun,Terengganu', 'amir123', '$2a$12$Bc/va.R34eM/NKJ1djCI2eQSSHu5ZVnRoQZ.YSwCS.RP3zoNTxbJq', 'admin'),
(2, 'Ahmad bin Ali', '060215119872', '0156729871', 'Jalan 12,Kubang Kerian,Kelantan', 'ahmad06', '$2a$12$yVnRe3dR4Vu8eggvVrYF7u7rEOVEh24QANmsjofAVkV5paL.cAOJu', 'customer'),
(3, 'Harith bin Ali', '030212110517', '0198725612', 'Lot 12,Kampung Durian Runtuh,Selangor', 'harithf', '$2a$12$6WRx29m6Elk8i2VdsoaKdeeYBXhpWXrL.ibD15mukBajnvVse6tWW', 'customer'),
(4, 'Amir Suihami bin Roslan', '980121110916', '01999999211', 'PT 778,Taman Megah Bersatu,Klang,Selangor', 'amir04', '$2a$12$A5KcV/LPpfqhG1AFO0mjuO5YazN0shg7AEvjioYyo6A6tLd09YfX.', 'customer'),
(5, 'HAirth', '010219621234', '0179821092', 'D-0-1,Blok A2,Kondominium Dahlia,Johor Bahru,Johor\r\n', 'harithpoyo', '$2a$12$09oNOBGmETMhW0MU63U7TuSu.jJBj96uxd3fgh7rtcn5vR1AghpcW', 'customer'),
(9, 'Salahuddin bin Ali', '010212019812', '0198382222', 'Lot 76,Lorong Haji Mat,Kemaman,Terengganu', 'salahuddin123', '$2a$12$um3hxskiNtKl7gdW6ehGvOWkecQnM/kiLnIzZNRnpKYBhZ42.xx7G', 'customer'),
(10, 'Aiman bin Hashim', '040567112341', '0197861234', 'Lot 7428,Wakaf  Che Husin,Terengganu ', 'aiman123', '$2a$12$DzdjpDenM0eLitCA2Ak3WupUHIffHuR/hMOHfrW8TboI9RNuzzY8y', 'customer'),
(11, 'Aiman bin Jaim', '040120110912', '0191234657', 'Lot 729,Wakaf Che Yer,Kelantan', 'aiman04', '$2a$12$f7JTGXd5llekCvZA9EkVU.4oIMcUs0R26.arrwq6MQrRqAIqv/8Nu', 'customer');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `book`
--
ALTER TABLE `book`
  ADD PRIMARY KEY (`BookID`);

--
-- Indexes for table `book_order`
--
ALTER TABLE `book_order`
  ADD PRIMARY KEY (`orderID`,`BookID`),
  ADD KEY `BookID` (`BookID`);

--
-- Indexes for table `order`
--
ALTER TABLE `order`
  ADD PRIMARY KEY (`orderID`),
  ADD KEY `UserID` (`UserID`);

--
-- Indexes for table `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`UserID`),
  ADD UNIQUE KEY `IC_no_UNQ` (`IC_no`),
  ADD UNIQUE KEY `cus_username_UNQ` (`username`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `book`
--
ALTER TABLE `book`
  MODIFY `BookID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=16;

--
-- AUTO_INCREMENT for table `order`
--
ALTER TABLE `order`
  MODIFY `orderID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;

--
-- AUTO_INCREMENT for table `user`
--
ALTER TABLE `user`
  MODIFY `UserID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `book_order`
--
ALTER TABLE `book_order`
  ADD CONSTRAINT `book_order_ibfk_1` FOREIGN KEY (`orderID`) REFERENCES `order` (`orderID`) ON DELETE CASCADE,
  ADD CONSTRAINT `book_order_ibfk_2` FOREIGN KEY (`BookID`) REFERENCES `book` (`BookID`) ON DELETE CASCADE;

--
-- Constraints for table `order`
--
ALTER TABLE `order`
  ADD CONSTRAINT `order_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `user` (`UserID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
