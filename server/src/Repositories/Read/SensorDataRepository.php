<?php

namespace App\Repositories\Read;

use PDO;
use PDOException;
use App\Exceptions\Connections\DatabaseException;

class SensorDataRepository
{
    private PDO $pdo;

    public function __construct(PDO $pdo)
    {
        $this->pdo = $pdo;
    }

    public function getLatestData()
    {
        return [
            "temperature" => $this->getLatestTemperature(),
            "humidity" => $this->getLatestHumidity(),
            "rain_status" => $this->getLatestRainStatus(),
            "gas_level" => $this->getLatestGasLevel()
        ];
    }

    private function getLatestTemperature(): ?array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  temperature, created_at FROM temperature ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $result = $stmt->fetch(PDO::FETCH_ASSOC);

            return $result ?: null;
        } catch (PDOException $error) {
            throw new DatabaseException('Failed to fetch temperature from database');
        }
    }

    private function getLatestHumidity(): ?array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  humidity, created_at FROM humidity ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $result = $stmt->fetch(PDO::FETCH_ASSOC);

            return $result ?: null;
        } catch (PDOException $error) {
            throw new DatabaseException('Failed to fetch humidity from database');
        }
    }

    private function getLatestRainStatus(): ?array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  is_raining, created_at FROM rain_status ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $result = $stmt->fetch(PDO::FETCH_ASSOC);

            return $result ?: null;
        } catch (PDOException $error) {
            throw new DatabaseException('Failed to fetch rain status from database');
        }
    }

    private function getLatestGasLevel(): ?array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  gas_level, created_at FROM gas_level ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $result = $stmt->fetch(PDO::FETCH_ASSOC);

            return $result ?: null;
        } catch (PDOException $error) {
            throw new DatabaseException('Failed to fetch gas level from database');
        }
    }
}