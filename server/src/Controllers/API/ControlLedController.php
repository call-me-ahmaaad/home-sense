<?php

namespace App\Controllers\API;

use App\MQTT\MqttPublish;
use App\Services\SystemLogger\StatusLogger;
use Exception;

class ControlLedController
{
    private MqttPublish $mqttPublish;
    private StatusLogger $statusLogger;

    public function __construct(MqttPublish $mqttPublish, StatusLogger $statusLogger)
    {
        $this->mqttPublish = $mqttPublish;
        $this->statusLogger = $statusLogger;
    }

    public function receiveData()
    {
        $message = json_decode(file_get_contents("php://input"), true);

        if (!$message) {
            http_response_code(400);

            echo json_encode([
                "success" => false,
                "reason" => "Invalid JSON payload",
                "message" => $message
            ]);

            return;
        }

        $led = $message['led'] ?? null;
        $state = $message['state'] ?? null;

        if (!$led || !$state) {
            echo json_encode([
                "success" => false,
                "reason" => "LED and state are required"
            ]);

            return;
        }

        try {
            $this->mqttPublish->publish($led, $state);

            echo json_encode([
                "success" => true,
                "message" => [
                    "led" => $led,
                    "state" => $state
                ]
            ]);

            $this->statusLogger->info("Successfully published message to topic", [
                "led" => $led,
                "state" => $state
            ]);

        } catch (Exception $error) {
            echo json_encode([
                "success" => false,
                "reason" => $error->getMessage()
            ]);

            $this->statusLogger->critical("Failed to publish message to topic", [
                "reason" => $error->getMessage(),
                "led" => $led,
                "state" => $state
            ]);
        }
    }
}