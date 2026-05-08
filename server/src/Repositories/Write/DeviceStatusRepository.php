<?php

namespace App\Repositories\Write;

use App\Exceptions\Connections\DatabaseException;
use App\Services\SystemLogger\StatusLogger;
use PDO;
use PDOException;

class DeviceStatusRepository
{
    private PDO $pdo;

    private StatusLogger $statusLogger;

    public function __construct(PDO $pdo, StatusLogger $statusLogger)
    {
        $this->pdo = $pdo;
        $this->statusLogger = $statusLogger;
    }

    public function insert(string $value)
    {
        $this->insertStatus($value);
    }

    private function insertStatus(string $value)
    {
        try {
            $stmt = $this->pdo->prepare("INSERT INTO device_status (status) VALUES (:status)");

            $stmt->execute([
                ":status" => $value
            ]);

            $this->statusLogger->info('Successfully insert status to database', [
                'status' => $value
            ]);
        } catch (PDOException $error) {
            $this->statusLogger->info('Failed to insert device status to table');

            throw new DatabaseException("Failed to insert device status to table");
        }
    }
}