<?php

namespace App\Controllers\MQTT;

use App\Repositories\Write\DeviceStatusRepository;
use Exception;

class DeviceStatusController
{
    private DeviceStatusRepository $deviceStatusRepository;

    public function __construct(DeviceStatusRepository $deviceStatusRepository)
    {
        $this->deviceStatusRepository = $deviceStatusRepository;
    }

    public function handle(string $message): void
    {
        $status = json_decode($message);

        try {
            $this->deviceStatusRepository->insert($status->status);
        } catch (Exception $error) {
            echo $error->getMessage() . PHP_EOL;
        }
    }
}