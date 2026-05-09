<?php

namespace App\Services;

use App\Exceptions\Data\{TemperatureException, HumidityException, GasException};
use App\Models\SensorData;

class SensorDataProcessor
{
    public function processor(float $temperature, float $humidity, bool $isRaining, float $gasLevel): SensorData
    {
        if ($temperature < -40 || $temperature > 80) {
            throw new TemperatureException("Temperature out of sensor range. Temperature: {$temperature}");
        }

        if ($humidity < 0 || $humidity > 100) {
            throw new HumidityException("Humidity out of sensor range. Humidity: {$humidity}");
        }

        if ($gasLevel < 0 || $gasLevel > 10000) {
            throw new GasException("Gas level out of sensor range. Gas level: {$gasLevel}");
        }

        return new SensorData($temperature, $humidity, $isRaining, $gasLevel);
    }
}