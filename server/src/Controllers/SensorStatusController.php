<?php

namespace App\Controllers;

use App\Repositories\SensorStatusRepository;
use Exception;

class SensorStatusController
{
    private SensorStatusRepository $sensorStatusRepository;

    public function __construct(SensorStatusRepository $sensorStatusRepository)
    {
        $this->sensorStatusRepository = $sensorStatusRepository;
    }

    public function handle(string $message): void
    {
        $status = json_decode($message);

        try {
            $this->sensorStatusRepository->insert((bool) $status->status);
        } catch (Exception $error) {
            echo $error->getMessage() . PHP_EOL;
        }
    }
}