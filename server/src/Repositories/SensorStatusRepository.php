<?php

namespace App\Repositories;

use App\Exceptions\Connections\DatabaseException;
use App\Services\SystemLogger\StatusLogger;
use PDO;
use PDOException;

class SensorStatusRepository
{
    private PDO $pdo;

    private StatusLogger $statusLogger;

    public function __construct(PDO $pdo, StatusLogger $statusLogger)
    {
        $this->pdo = $pdo;
        $this->statusLogger = $statusLogger;
    }

    public function insert(bool $value)
    {
        $this->insertStatus($value);
    }

    private function insertStatus(bool $value)
    {
        try {
            $stmt = $this->pdo->prepare("INSERT INTO device_status (status) VALUES (:status)");

            $stmt->execute([
                ":status" => (int) $value
            ]);

            $this->statusLogger->info('Succesfully insert status to database', [
                'status' => $value
            ]);
        } catch (PDOException $error) {
            $this->statusLogger->info('Failed to insert device status to table');

            throw new DatabaseException("[ERROR][INSERT ERROR] Failed to insert device status to table!");
        }
    }
}