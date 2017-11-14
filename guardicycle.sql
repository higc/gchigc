-- phpMyAdmin SQL Dump
-- version 4.7.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- 생성 시간: 17-11-10 09:20
-- 서버 버전: 10.1.26-MariaDB
-- PHP 버전: 7.1.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 데이터베이스: `guardicycle`
--

-- --------------------------------------------------------

--
-- 테이블 구조 `driveinfo`
--

CREATE TABLE `driveinfo` (
  `date` date NOT NULL,
  `accum_time` time NOT NULL,
  `max_velocity` double(10,3) NOT NULL,
  `distance` double(10,3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- 테이블 구조 `falldown`
--

CREATE TABLE `falldown` (
  `date` date NOT NULL,
  `count` int(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- 테이블 구조 `location`
--

CREATE TABLE `location` (
  `date` date NOT NULL,
  `time` time NOT NULL,
  `latitude` double(16,13) NOT NULL,
  `longitude` double(16,13) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
ALTER TABLE `driveinfo`
  ADD PRIMARY KEY (`date`);

--
-- 테이블의 인덱스 `falldown`
--
ALTER TABLE `falldown`
  ADD PRIMARY KEY (`date`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
