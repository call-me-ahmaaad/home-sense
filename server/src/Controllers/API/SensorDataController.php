<?php

namespace App\Controllers\API;

use App\Repositories\Read\SensorDataRepository;

class SensorDataController
{
    private SensorDataRepository $sensorDataRepository;

    public function __construct(SensorDataRepository $sensorDataRepository)
    {
        $this->sensorDataRepository = $sensorDataRepository;
    }

    public function getLatestData()
    {
        header("Content-Type: application/json");

        $latestData = $this->sensorDataRepository->getLatestData();

        echo json_encode([
            "success" => true,
            "data" => $latestData
        ]);
    }
}