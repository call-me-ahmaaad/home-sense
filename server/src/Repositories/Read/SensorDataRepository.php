<?php

namespace App\Repositories\Read;

use PDO;
use PDOException;
use App\Services\SystemLogger\SensorDataLogger;
use App\Exceptions\Connections\DatabaseException;

class SensorDataRepository
{
    private PDO $pdo;
    private SensorDataLogger $sensorDataLogger;

    public function __construct(PDO $pdo, SensorDataLogger $sensorDataLogger)
    {
        $this->pdo = $pdo;
        $this->sensorDataLogger = $sensorDataLogger;
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

    private function getLatestTemperature(): array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  temperature, created_at FROM temperature ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $this->sensorDataLogger->info('Successfully fetch temperature from database');

            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to fetch temperature from database');

            throw new DatabaseException('Failed to fetch temperature from database');
        }
    }

    private function getLatestHumidity(): array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  humidity, created_at FROM humidity ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $this->sensorDataLogger->info('Successfully fetch humidity from database');

            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to fetch humidity from database');

            throw new DatabaseException('Failed to fetch humidity from database');
        }

    }

    private function getLatestRainStatus(): array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  is_raining, created_at FROM rain_status ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $this->sensorDataLogger->info('Successfully fetch rain status from database');

            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to fetch rain status from database');

            throw new DatabaseException('Failed to fetch rain status from database');
        }

    }

    private function getLatestGasLevel(): array
    {
        try {
            $stmt = $this->pdo->prepare("SELECT id,  gas_level, created_at FROM gas_level ORDER BY id DESC LIMIT 1");
            $stmt->execute();

            $this->sensorDataLogger->info('Successfully fetch gas level from database');

            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to fetch gas level from database');

            throw new DatabaseException('Failed to fetch gas level from database');
        }

    }
}