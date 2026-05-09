<?php

namespace App\Repositories\Read;

use App\Exceptions\Connections\DatabaseException;
use PDO;
use PDOException;

class DeviceStatusRepository
{
    private PDO $pdo;

    public function __construct(PDO $pdo)
    {
        $this->pdo = $pdo;
    }

    public function getLatestData()
    {
        return $this->getLatestDeviceStatus();
    }

    private function getLatestDeviceStatus()
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  status, created_at FROM device_status ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $error) {
            throw new DatabaseException('Failed to fetch device status from database!');
        }
    }
}