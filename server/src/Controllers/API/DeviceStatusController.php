<?php

namespace App\Controllers\API;

use App\Repositories\Read\DeviceStatusRepository;

class DeviceStatusController
{
    private DeviceStatusRepository $deviceStatusRepository;

    public function __construct(DeviceStatusRepository $deviceStatusRepository)
    {
        $this->deviceStatusRepository = $deviceStatusRepository;
    }

    public function getLatestData()
    {
        header("Content-Type: application/json");

        $latestData = $this->deviceStatusRepository->getLatestData();

        echo json_encode([
            "success" => true,
            "data" => $latestData
        ]);
    }
}