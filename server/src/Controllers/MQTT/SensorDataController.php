<?php

namespace App\Controllers\MQTT;

use App\Repositories\Write\SensorDataRepository;
use App\Services\SensorDataProcess;
use Exception;

class SensorDataController
{
    private SensorDataProcess $sensorDataProcess;
    private SensorDataRepository $sensorDataRepository;

    public function __construct(SensorDataProcess $sensorDataProcess, SensorDataRepository $sensorDataRepository)
    {
        $this->sensorDataProcess = $sensorDataProcess;
        $this->sensorDataRepository = $sensorDataRepository;
    }

    public function handle(string $message): void
    {
        $data = json_decode($message);

        try {
            $sensorData = $this->sensorDataProcess->process(
                $data->temperature,
                $data->humidity,
                (bool) $data->rain_status,
                $data->gas_level
            );

            $this->sensorDataRepository->insert($sensorData);
        } catch (Exception $error) {
            echo $error->getMessage() . PHP_EOL;
        }
    }
}