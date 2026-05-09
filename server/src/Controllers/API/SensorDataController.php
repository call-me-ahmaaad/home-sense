<?php

namespace App\Controllers\API;

use App\Repositories\Read\SensorDataRepository;
use Exception;

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

        try {
            $latestData = $this->sensorDataRepository->getLatestData();

            echo json_encode([
                "success" => true,
                "data" => $latestData
            ]);
        } catch (Exception $error) {
            http_response_code(500);

            echo json_encode([
                "success" => false,
                "reason" => $error->getMessage()
            ]);
        }
    }
}