<?php

namespace App\Controllers\MQTT;

use App\Repositories\Write\SensorDataRepository;
use App\Services\SensorDataProcessor;
use App\Services\SystemLogger\SensorDataLogger;
use Exception;

class SensorDataController
{
    private SensorDataProcessor $sensorDataProcess;
    private SensorDataRepository $sensorDataRepository;
    private SensorDataLogger $sensorDataLogger;

    public function __construct(SensorDataProcessor $sensorDataProcess, SensorDataRepository $sensorDataRepository, SensorDataLogger $sensorDataLogger)
    {
        $this->sensorDataProcess = $sensorDataProcess;
        $this->sensorDataRepository = $sensorDataRepository;
        $this->sensorDataLogger = $sensorDataLogger;
    }

    public function handle(string $message): void
    {
        $data = json_decode($message);

        try {
            $sensorData = $this->sensorDataProcess->processor(
                $data->temperature,
                $data->humidity,
                (bool) $data->rain_status,
                $data->gas_level
            );

            $this->sensorDataRepository->insert($sensorData);
        } catch (Exception $error) {
            $this->sensorDataLogger->error("Failed to validate and process sensor data", [
                "message" => $error->getMessage()
            ]);
        }
    }
}