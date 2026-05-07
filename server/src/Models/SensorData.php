<?php

namespace App\Models;

class SensorData
{
    private float $temperature;
    private float $humidity;
    private bool $isRaining;
    private float $gasLevel;

    public function __construct(float $temperature, float $humidity, bool $isRaining, float $gasLevel)
    {
        $this->temperature = $temperature;
        $this->humidity = $humidity;
        $this->isRaining = $isRaining;
        $this->gasLevel = $gasLevel;
    }

    public function getTemperature(): float
    {
        return $this->temperature;
    }

    public function getHumidity(): float
    {
        return $this->humidity;
    }

    public function getRainStatus(): bool
    {
        return $this->isRaining;
    }

    public function getGasLevel(): float
    {
        return $this->gasLevel;
    }
}