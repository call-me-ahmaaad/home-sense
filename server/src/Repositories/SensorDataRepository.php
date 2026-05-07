<?php

namespace App\Repositories;

use App\Models\SensorData;
use App\Exceptions\Connections\DatabaseException;
use App\Services\SystemLogger\SensorDataLogger;
use PDO;
use PDOException;

class SensorDataRepository
{
    private PDO $pdo;
    private SensorDataLogger $sensorDataLogger;

    public function __construct(PDO $pdo, SensorDataLogger $sensorDataLogger)
    {
        $this->pdo = $pdo;
        $this->sensorDataLogger = $sensorDataLogger;
    }

    public function insert(SensorData $sensorData): void
    {
        $this->insertTemperature($sensorData->getTemperature());
        $this->insertHumidity($sensorData->getHumidity());
        $this->insertRainStatus($sensorData->getRainStatus());
        $this->insertGasLevel($sensorData->getGasLevel());
    }

    private function insertTemperature(float $value): void
    {
        try {
            $stmt = $this->pdo->prepare("INSERT INTO temperature (temperature) VALUES (:temperature)");

            $stmt->execute([
                ":temperature" => $value
            ]);

            $this->sensorDataLogger->info('Succesfully insert temperature to database', [
                'temperature' => $value
            ]);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to insert temperature to table');

            throw new DatabaseException("[ERROR][INSERT ERROR] Failed to insert temperature to table!");
        }
    }

    private function insertHumidity(float $value): void
    {
        try {
            $stmt = $this->pdo->prepare("INSERT INTO humidity (humidity) VALUES (:humidity)");

            $stmt->execute([
                ":humidity" => $value
            ]);

            $this->sensorDataLogger->info('Succesfully insert humidity to database', [
                'humidity' => $value
            ]);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to insert humidity to table');

            throw new DatabaseException("[ERROR][INSERT ERROR] Failed to insert humidity to table!");
        }
    }

    private function insertRainStatus(bool $value): void
    {
        try {
            $stmt = $this->pdo->prepare("INSERT INTO rain_status (is_raining) VALUES (:rain_status)");

            $stmt->execute([
                ":rain_status" => (int) $value
            ]);

            $this->sensorDataLogger->info('Succesfully insert rain_status to database', [
                'rain_status' => $value
            ]);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to insert rain status to table');

            throw new DatabaseException("[ERROR][INSERT ERROR] Failed to insert rain_status to table {$value}!");
        }
    }

    private function insertGasLevel(float $value): void
    {
        try {
            $stmt = $this->pdo->prepare("INSERT INTO gas_level (gas_level) VALUES (:gas_level)");

            $stmt->execute([
                ":gas_level" => $value
            ]);

            $this->sensorDataLogger->info('Succesfully insert gas level to database', [
                'gas_level' => $value
            ]);
        } catch (PDOException $error) {
            $this->sensorDataLogger->error('Failed to insert gas level to table');

            throw new DatabaseException("[ERROR][INSERT ERROR] Failed to insert gas level to table!");
        }
    }
}