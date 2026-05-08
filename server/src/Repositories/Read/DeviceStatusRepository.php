<?php

namespace App\Repositories\Read;

use App\Exceptions\Connections\DatabaseException;
use PDO;
use PDOException;
use App\Services\SystemLogger\StatusLogger;

class DeviceStatusRepository
{
    private PDO $pdo;
    private StatusLogger $statusLogger;

    public function __construct(PDO $pdo, StatusLogger $statusLogger)
    {
        $this->pdo = $pdo;
        $this->statusLogger = $statusLogger;
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

            $this->statusLogger->info('Successfully fetch device status from database');

            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $error) {
            $this->statusLogger->error('Failed to fetch device status from database');

            throw new DatabaseException('Failed to fetch device status from database!');
        }
    }
}