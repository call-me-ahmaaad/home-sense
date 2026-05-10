<?php

namespace App\Controllers\API;

use App\Repositories\Read\DeviceStatusRepository;
use Exception;

class DeviceStatusController
{
    private DeviceStatusRepository $deviceStatusRepository;

    public function __construct(DeviceStatusRepository $deviceStatusRepository)
    {
        $this->deviceStatusRepository = $deviceStatusRepository;
    }

    public function getLatestData()
    {
        try {
            $latestData = $this->deviceStatusRepository->getLatestData();

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