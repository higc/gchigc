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
-- 테이블의 덤프 데이터 `driveinfo`
--

INSERT INTO `driveinfo` (`date`, `accum_time`, `max_velocity`, `distance`) VALUES
('2017-10-19', '00:13:24', 21.325, 1.300),
('2017-10-21', '00:42:09', 24.741, 3.928),
('2017-10-22', '00:33:24', 22.539, 2.740),
('2017-10-25', '01:08:50', 19.027, 5.104),
('2017-10-27', '00:19:05', 21.113, 1.839),
('2017-11-08', '00:06:09', 23.450, 2.230);

-- --------------------------------------------------------

--
-- 테이블 구조 `falldown`
--

CREATE TABLE `falldown` (
  `date` date NOT NULL,
  `count` int(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 테이블의 덤프 데이터 `falldown`
--

INSERT INTO `falldown` (`date`, `count`) VALUES
('2017-10-19', 3),
('2017-10-21', 5),
('2017-10-22', 2),
('2017-10-25', 6),
('2017-10-27', 2),
('2017-11-08', 3);

-- --------------------------------------------------------

--
-- 테이블 구조 `location`
--

CREATE TABLE `location` (
  `date` date NOT NULL,
  `time` time NOT NULL,
  `latitude` double(16,13) NOT NULL,
  `longitude` double(16,13) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 테이블의 덤프 데이터 `location`
--

INSERT INTO `location` (`date`, `time`, `latitude`, `longitude`) VALUES
('2017-10-19', '14:26:20', 37.5665016174316, 126.9484405517578),
('2017-10-19', '14:26:56', 37.5665016174316, 126.9484405517578),
('2017-10-19', '14:27:18', 37.5664710998535, 126.9484558105469),
('2017-10-19', '14:27:42', 37.5666657238974, 126.9479587592839),
('2017-10-19', '14:28:23', 37.5667250285731, 126.9478459947284),
('2017-11-03', '17:12:56', 37.5638889540392, 126.9381090976358),
('2017-11-03', '17:15:24', 37.5630808785032, 126.9359531475389),
('2017-11-03', '17:19:48', 37.5615617783675, 126.9364684983042),
('2017-11-03', '17:23:09', 37.5602869405834, 126.9366061254734),
('2017-11-03', '17:27:31', 37.5586790907465, 126.9368424563492),
('2017-11-03', '17:31:12', 37.5576507936402, 126.9367993846553),
('2017-11-03', '17:34:54', 37.5568933304929, 126.9362639944723),
('2017-11-03', '17:36:44', 37.5559660000000, 126.9364560000000);

--
-- 덤프된 테이블의 인덱스
--

--
-- 테이블의 인덱스 `driveinfo`
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
