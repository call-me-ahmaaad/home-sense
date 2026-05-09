<?php

namespace App\MQTT;

use App\Services\SystemLogger\StatusLogger;
use Bluerhinos\phpMQTT;

class MqttPublish
{
    private phpMQTT $mqttConnection;
    private StatusLogger $statusLogger;

    public function __construct(phpMQTT $mqttConnection, StatusLogger $statusLogger)
    {
        $this->mqttConnection = $mqttConnection;
        $this->statusLogger = $statusLogger;
    }

    public function publish($led, $state)
    {
        $topic = "home_sense/led/{$led}";

        $this->mqttConnection->publish($topic, $state, 0);

        $this->statusLogger->info("Successfully sent message to topic");
    }
}