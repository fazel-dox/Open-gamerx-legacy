-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Sep 03, 2025 at 01:53 AM
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
-- Database: `gamerx_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin_logs`
--

CREATE TABLE `admin_logs` (
  `id` int(11) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp(),
  `admin_name` varchar(24) NOT NULL,
  `action` varchar(512) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `bans`
--

CREATE TABLE `bans` (
  `id` int(11) NOT NULL,
  `banned_name` varchar(24) NOT NULL,
  `banned_ip` varchar(45) DEFAULT NULL,
  `admin_name` varchar(24) NOT NULL,
  `reason` varchar(128) NOT NULL,
  `banned_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `ctf_events`
--

CREATE TABLE `ctf_events` (
  `id` int(11) NOT NULL,
  `Name` varchar(100) NOT NULL,
  `ObjectSet` varchar(100) DEFAULT NULL,
  `CreatorName` varchar(24) DEFAULT NULL,
  `RedPickupX` float NOT NULL,
  `RedPickupY` float NOT NULL,
  `RedPickupZ` float NOT NULL,
  `BluePickupX` float NOT NULL,
  `BluePickupY` float NOT NULL,
  `BluePickupZ` float NOT NULL,
  `RedTeleportX` float NOT NULL,
  `RedTeleportY` float NOT NULL,
  `RedTeleportZ` float NOT NULL,
  `RedTeleportFA` float NOT NULL,
  `BlueTeleportX` float NOT NULL,
  `BlueTeleportY` float NOT NULL,
  `BlueTeleportZ` float NOT NULL,
  `BlueTeleportFA` float NOT NULL,
  `Weapon1` int(11) DEFAULT 0,
  `Weapon2` int(11) DEFAULT 0,
  `Weapon3` int(11) DEFAULT 0,
  `Weapon4` int(11) DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `players`
--

CREATE TABLE `players` (
  `id` int(11) NOT NULL,
  `Name` varchar(24) NOT NULL,
  `Password` varchar(255) NOT NULL,
  `Level` int(11) DEFAULT 1,
  `Money` int(11) DEFAULT 5000,
  `Kills` int(11) DEFAULT 0,
  `Deaths` int(11) DEFAULT 0,
  `AdminLevel` int(3) DEFAULT 0,
  `LastIP` varchar(45) DEFAULT NULL,
  `CreatedAt` timestamp NOT NULL DEFAULT current_timestamp(),
  `LastSeen` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `admin_logs`
--
ALTER TABLE `admin_logs`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `bans`
--
ALTER TABLE `bans`
  ADD PRIMARY KEY (`id`),
  ADD KEY `banned_name_idx` (`banned_name`),
  ADD KEY `banned_ip_idx` (`banned_ip`);

--
-- Indexes for table `ctf_events`
--
ALTER TABLE `ctf_events`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `Name_UNIQUE` (`Name`);

--
-- Indexes for table `players`
--
ALTER TABLE `players`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `Name_UNIQUE` (`Name`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `admin_logs`
--
ALTER TABLE `admin_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `bans`
--
ALTER TABLE `bans`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `ctf_events`
--
ALTER TABLE `ctf_events`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `players`
--
ALTER TABLE `players`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
