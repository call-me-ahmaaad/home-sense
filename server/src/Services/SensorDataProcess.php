<?php

namespace App\Services;

use App\Exceptions\Data\{HumidityException, GasException};
use App\Models\SensorData;

class SensorDataProcess
{
    public function process(float $temperature, float $humidity, bool $isRaining, float $gasLevel): SensorData
    {
        if ($humidity > 100) {
            throw new HumidityException("[ERROR][RANGE ERROR] Humidity value should not exceed 100");
        }

        if ($humidity < 0) {
            throw new HumidityException("[ERROR][RANGE ERROR] Humidity value must be positive");
        }

        if ($gasLevel < 0) {
            throw new GasException("[ERROR][RANGE ERROR] Gas level value must be positive");
        }

        return new SensorData($temperature, $humidity, $isRaining, $gasLevel);
    }
}