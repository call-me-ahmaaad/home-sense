<?php

namespace App\MQTT;

use App\Exceptions\Connections\MqttException;
use Bluerhinos\phpMQTT;
use App\Controllers\SensorDataController;
use App\Controllers\SensorStatusController;
use Exception;
use App\Services\SystemLogger\StatusLogger;

class MqttSubscriber
{
    private phpMQTT $mqttConnection;
    private SensorDataController $sensorDataController;
    private SensorStatusController $sensorStatusController;
    private StatusLogger $statusLogger;

    public function __construct(phpMQTT $mqttConnection, SensorDataController $sensorDataController, SensorStatusController $sensorStatusController, StatusLogger $statusLogger)
    {
        $this->mqttConnection = $mqttConnection;
        $this->sensorDataController = $sensorDataController;
        $this->sensorStatusController = $sensorStatusController;
        $this->statusLogger = $statusLogger;
    }

    public function subscribe(): void
    {
        $topics = [
            "home_sense/C8F09E9AED08/data" => [
                "qos" => 0,
                "function" => function ($topic, $message) {
                    $this->sensorDataController->handle($message);
                }
            ],
            "home_sense/C8F09E9AED08/status" => [
                "qos" => 0,
                "function" => function ($topic, $message) {
                    $this->sensorStatusController->handle($message);
                }
            ]
        ];

        try {
            $this->mqttConnection->subscribe($topics, 0);

            $this->statusLogger->info("Successfully subscribed to topic");

            while ($this->mqttConnection->proc()) {

            }
        } catch (Exception $error) {
            $this->statusLogger->critical('Failed to subscribe to topic');

            throw new MqttException("[ERROR][CONNECTION ERROR] Failed to subscribe to topic");
        }
    }
}