<?php

namespace App\Controllers\MQTT;

use App\Repositories\Write\DeviceStatusRepository;
use App\Services\SystemLogger\StatusLogger;
use Exception;

class DeviceStatusController
{
    private DeviceStatusRepository $deviceStatusRepository;
    private StatusLogger $statusLogger;

    public function __construct(DeviceStatusRepository $deviceStatusRepository, StatusLogger $statusLogger)
    {
        $this->deviceStatusRepository = $deviceStatusRepository;
        $this->statusLogger = $statusLogger;
    }

    public function handle(string $message): void
    {
        $status = json_decode($message);

        try {
            $this->deviceStatusRepository->insert($status->status);
        } catch (Exception $error) {
            $this->statusLogger->error("Failed to process device status", [
                "message" => $error->getMessage()
            ]);
        }
    }
}