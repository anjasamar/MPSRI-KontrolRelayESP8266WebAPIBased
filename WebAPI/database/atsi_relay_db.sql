-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Jun 01, 2022 at 06:17 PM
-- Server version: 5.7.36
-- PHP Version: 7.4.26

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `atsi_relay_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `tabel_kontrol`
--

DROP TABLE IF EXISTS `tabel_kontrol`;
CREATE TABLE IF NOT EXISTS `tabel_kontrol` (
  `ID` int(20) NOT NULL AUTO_INCREMENT,
  `DEVICE` varchar(20) NOT NULL,
  `BOARD` varchar(20) NOT NULL,
  `GPIO` int(20) NOT NULL,
  `STATE` int(20) NOT NULL,
  `TYPE` enum('Active High','Active Low') NOT NULL,
  `LOGTIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=43 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tabel_kontrol`
--

INSERT INTO `tabel_kontrol` (`ID`, `DEVICE`, `BOARD`, `GPIO`, `STATE`, `TYPE`, `LOGTIME`) VALUES
(41, 'Saklar 1', '1', 14, 0, 'Active High', '2022-06-01 18:09:00'),
(42, 'Saklar 2', '1', 12, 0, 'Active High', '2022-06-01 18:09:55');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
